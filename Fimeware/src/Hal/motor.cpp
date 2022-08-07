#include <Hal/motor.h>
#include <main.h>
#include <display.h>
#include <SimpleFOC.h>
#include "mt6701_sensor.h"

#include "util.h"

// 死区制动百分率
static const float DEAD_ZONE_DETENT_PERCENT = 0.2;
// 死区RAD?
static const float DEAD_ZONE_RAD = 1 * _PI / 180;

// 怠速速度ewma alpha
static const float IDLE_VELOCITY_EWMA_ALPHA = 0.001;
// 怠速速度每秒钟弧度
static const float IDLE_VELOCITY_RAD_PER_SEC = 0.05;
// 怠速修正延迟millis
static const uint32_t IDLE_CORRECTION_DELAY_MILLIS = 500;
// 怠速校正最大角度rad
static const float IDLE_CORRECTION_MAX_ANGLE_RAD = 5 * PI / 180;
// 怠速修正率
static const float IDLE_CORRECTION_RATE_ALPHA = 0.0005;

// 先设置一个默认值，等消息队列发送状态过来
KnobConfig motor_config = {
    .num_positions = 0,
    .position = 0,
    .position_width_radians = 8.225806452 * _PI / 180,
    .detent_strength_unit = 4,
    .endstop_strength_unit = 1,
    .snap_point = 1.1,
};

// 设置电机(极对数)
BLDCMotor motor = BLDCMotor(1);
BLDCDriver6PWM driver = BLDCDriver6PWM(PIN_UH, PIN_UL, PIN_VH, PIN_VL, PIN_WH, PIN_WL);

MT6701Sensor encoder = MT6701Sensor();

void update_motor_status(MOTOR_RUNNING_MODE_E motor_status)
{
    struct _knob_message *send_message;
    send_message = &MOTOR_MSG;
    send_message->ucMessageID = motor_status;
    xQueueSend(motor_msg_Queue, &send_message, (TickType_t)0);
}

void update_motor_config(int status)
{
    motor_config = super_knob_configs[status];
}

void motor_shake(int strength, int delay_time)
{
    motor.move(strength);
    for (int i = 0; i < delay_time; i++)
    {
        motor.loopFOC();
        vTaskDelay(1);
    }
    motor.move(-strength);
    for (int i = 0; i < delay_time; i++)
    {
        motor.loopFOC();
        vTaskDelay(1);
    }
}

int get_motor_position(void)
{
    return motor_config.position;
}

int get_motor_position_unit(void)
{
    return motor_config.sub_position_unit;
}

void Task_foc(void *pvParameters)
{
    (void)pvParameters;

    update_motor_status(MOTOR_INIT);

    // 设置驱动器电压
    driver.voltage_power_supply = 5;
    // 驱动器初始化
    driver.init();

#if SENSOR_MT6701
    encoder.init();
#endif
    // 编码器与电机连接
    motor.linkDriver(&driver);
    motor.pole_pairs = 7;

    // 设置控制环类型：先设置为角度控制，方便初次定位
    motor.controller = MotionControlType::angle;
    motor.voltage_limit = 5;
    motor.velocity_limit = 10000;
    // 传感器与电机连接
    motor.linkSensor(&encoder);

    float zero_electric_offset = 3.66;
    // float zero_electric_offset = 7.34;
    // 控制环 PID控制器
    Direction foc_direction = Direction::CW;
    motor.PID_velocity.P = 4;
    motor.PID_velocity.I = 0;
    motor.PID_velocity.D = 0.04;
    motor.PID_velocity.output_ramp = 10000; // 输出值的最大变化速度
    motor.PID_velocity.limit = 10;          // 位置控制的最大速度
    // motor.zero_electric_angle = zero_electric_offset;

    // 电机参数监控输出串口
    // motor.useMonitoring(Serial);
    // 电机初始化
    motor.init();

    encoder.update();
    vTaskDelay(10);

    motor.initFOC(zero_electric_offset, foc_direction);
    update_motor_status(MOTOR_INIT_SUCCESS);
    Serial.print("motor init success!!\n");

    // 设置控制环类型
    motor.controller = MotionControlType::torque;
    motor.loopFOC();
    motor.monitor_downsample = 0;
    update_motor_status(MOTOR_INIT_END);

    // 当前相对位置
    float current_detent_center = 0;
    // 上次空闲开始状态
    uint32_t last_idle_start = 0;
    // 怠速检查速度
    float idle_check_velocity_ewma = 0;

    uint32_t last_publish = 0;

    for (;;)
    {
        motor.loopFOC();
        //监听页面状态
        struct _knob_message *lvgl_message;

        if (xQueueReceive(motor_rcv_Queue, &(lvgl_message), (TickType_t)0))
        {
            Serial.printf("motor_rcv_Queue --->(%d)\n", lvgl_message->ucMessageID);

            switch (lvgl_message->ucMessageID)
            {
            // 更新
            case CHECKOUT_PAGE:
            {
                // 上次的相对位置
                current_detent_center = motor.shaft_angle;
#if SK_INVERT_ROTATION
                current_detent_center = -motor.shaft_angle;
#endif

                const float derivative_lower_strength = motor_config.detent_strength_unit * 0.08;
                const float derivative_upper_strength = motor_config.detent_strength_unit * 0.02;
                const float derivative_position_width_lower = radians(3);
                const float derivative_position_width_upper = radians(8);
                const float raw = derivative_lower_strength + (derivative_upper_strength - derivative_lower_strength) / (derivative_position_width_upper - derivative_position_width_lower) * (motor_config.position_width_radians - derivative_position_width_lower);

                // CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
                motor.PID_velocity.D = CLAMP(
                    raw,
                    min(derivative_lower_strength, derivative_upper_strength),
                    max(derivative_lower_strength, derivative_upper_strength));

                // // 存在页面切换 就震动一下
                // motor_shake(2, 2);
            }
            break;
            // 动作——按下
            case BUTTON_CLICK:
                motor_shake(2, 2);
                break;
            default:
                break;
            }
        }

        idle_check_velocity_ewma = motor.shaft_velocity * IDLE_VELOCITY_EWMA_ALPHA + idle_check_velocity_ewma * (1 - IDLE_VELOCITY_EWMA_ALPHA);
        if (fabsf(idle_check_velocity_ewma) > IDLE_VELOCITY_RAD_PER_SEC)
        {
            last_idle_start = 0;
        }
        else
        {
            if (last_idle_start == 0)
            {
                last_idle_start = millis();
            }
        }

        // 如果我们没有移动，并且我们接近中心(但不是完全在那里)，慢慢调整中心点以匹配当前位置
        // If we are not moving and we're close to the center (but not exactly there), slowly adjust the centerpoint to match the current position
        if (last_idle_start > 0 && millis() - last_idle_start > IDLE_CORRECTION_DELAY_MILLIS && fabsf(motor.shaft_angle - current_detent_center) < IDLE_CORRECTION_MAX_ANGLE_RAD)
        {
            // Serial.println("slowly adjust the centerpoint to match the current position......");
            current_detent_center = motor.shaft_angle * IDLE_CORRECTION_RATE_ALPHA + current_detent_center * (1 - IDLE_CORRECTION_RATE_ALPHA);
        }

        //到控制中心的角度 差值
        float angle_to_detent_center = motor.shaft_angle - current_detent_center;
#if SK_INVERT_ROTATION
        angle_to_detent_center = -motor.shaft_angle - current_detent_center;
#endif
        if (angle_to_detent_center > motor_config.position_width_radians * motor_config.snap_point && (motor_config.num_positions <= 0 || motor_config.position > 0))
        {
            current_detent_center += motor_config.position_width_radians;
            angle_to_detent_center -= motor_config.position_width_radians;
            motor_config.position--;
        }
        else if (angle_to_detent_center < -motor_config.position_width_radians * motor_config.snap_point && (motor_config.num_positions <= 0 || motor_config.position < motor_config.num_positions - 1))
        {
            current_detent_center -= motor_config.position_width_radians;
            angle_to_detent_center += motor_config.position_width_radians;
            motor_config.position++;
        }

        // CLAMP可以将随机变化的值限制在一个给定的区间[min,max]内
        //死区调整
        float dead_zone_adjustment = CLAMP(
            angle_to_detent_center,
            fmaxf(-motor_config.position_width_radians * DEAD_ZONE_DETENT_PERCENT, -DEAD_ZONE_RAD),
            fminf(motor_config.position_width_radians * DEAD_ZONE_DETENT_PERCENT, DEAD_ZONE_RAD));

        //出界
        bool out_of_bounds = motor_config.num_positions > 0 && ((angle_to_detent_center > 0 && motor_config.position == 0) || (angle_to_detent_center < 0 && motor_config.position == motor_config.num_positions - 1));
        motor.PID_velocity.limit = out_of_bounds ? 10 : 3;
        motor.PID_velocity.P = out_of_bounds ? motor_config.endstop_strength_unit * 4 : motor_config.detent_strength_unit * 4;

        //处理float类型的取绝对值
        if (fabsf(motor.shaft_velocity) > 60)
        {
            //如果速度太高 则不增加扭矩
            // Don't apply torque if velocity is too high (helps avoid positive feedback loop/runaway)
            Serial.println("(motor.shaft_velocity) > 60 !!!");
            motor.move(0);
        }
        else
        {
            float torque = motor.PID_velocity(-angle_to_detent_center + dead_zone_adjustment);
#if SK_INVERT_ROTATION
            torque = -torque;
#endif
            motor.move(torque);
        }

        if (millis() - last_publish > 10)
        {
            motor_config.sub_position_unit = -angle_to_detent_center / motor_config.position_width_radians,       
            last_publish = millis();
        }

        // Serial.println(motor_config.position);
        vTaskDelay(2);
    }
}

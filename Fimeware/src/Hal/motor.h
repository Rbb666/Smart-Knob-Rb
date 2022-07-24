#pragma once
#include <Arduino.h>

struct KnobConfig
{
    //可以运动的个数
    int32_t num_positions;
    //位置
    int32_t position;
    //位置宽度弧度 或者是每一步的度数
    float position_width_radians;
    //制动强度
    float detent_strength_unit;
    // end stop强度
    float endstop_strength_unit;
    //中断点
    float snap_point;
    //描述符
    char descriptor[50];
};
extern KnobConfig motor_config;

typedef enum
{
    MOTOR_INIT,
    MOTOR_INIT_SUCCESS,
    MOTOR_INIT_END,
    DEV_WORK_MODE,
    DEV_BLE_WORK,
} MOTOR_RUNNING_MODE_E;

static KnobConfig super_knob_configs[] = {
    {
        0,
        0,
        10 * PI / 180,
        0,
        1,
        1.1,
        "Unbounded\nNo detents", // 无限制 不制动
    },
    {
        80,
        0,
        10 * PI / 180,
        3,
        1,
        1.1,
        "Coarse values\nStrong detents", // 无限制 强阻尼
    },
    {
        0,
        0,
        10 * PI / 180,
        3,
        1,
        1.1,
        "Coarse values\nStrong detents", // 粗糙的棘轮 强阻尼
    },
    {
        256,
        127,
        1 * PI / 180,
        1,
        1,
        1.1,
        "Fine values\nWith detents", // 任意运动的控制  有阻尼 类似于机械旋钮
    },
    {
        256,
        127,
        1 * PI / 180,
        0,
        1,
        1.1,
        "Fine values\nNo detents", // 任意运动的控制  无阻尼
    },
    {
        2, //可以运动的个数
        0,
        60 * PI / 180, // 每一步60度
        1,             // 制动强度为1
        1,
        0.55,                    // Note the snap point is slightly past the midpoint (0.5); compare to normal detents which use a snap point *past* the next value (i.e. > 1)
        "On/off\nStrong detent", // 模拟开关  强制动
    },
};

int get_motor_position(void);
void update_motor_config(int status);
void Task_foc(void *pvParameters);

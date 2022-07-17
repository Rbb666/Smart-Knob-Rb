#include <main.h>
#include <Hal/motor.h>
#include <display.h>
#include "interface_task.h"

#define LVGL_RUNNING_CORE 0  // 内核选择   LVGL用核心0  其他用核心1
#define ESP32_RUNNING_CORE 1 // 内核选择   LVGL用核心0  其他用核心1

TaskHandle_t Task_foc_Handle;       // foc task
TaskHandle_t Task_lvgl_Handle;      // lvgl task
TaskHandle_t Task_interface_Handle; // interface task

QueueHandle_t motor_msg_Queue = NULL; // lvgl 接收消息队列
QueueHandle_t motor_rcv_Queue = NULL; // motor 接收消息队列

SemaphoreHandle_t xBinarySemaphore = NULL; // 二值信号量

_knob_message LVGL_MSG;
_knob_message MOTOR_MSG;

InterfaceTask interface_task = InterfaceTask(0);

void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);

  // Serial.printf("    _________                      __             ____  __.            ___.      \n");
  // Serial.printf("  /   _____/ _____ _____ ________/  |_          |    |/ _| ____   _____|_ |__    \n");
  // Serial.printf("  \_____  \ /     \__   \_  __ \   __\  ______  |      <  /    \ /  _ \| __  \   \n");
  // Serial.printf("  /        \  Y Y  \/ __ \|  | \/|  |   /_____/ |    |  \|   |  (  <_> ) \_\  \  \n");
  // Serial.printf(" /_________/__|_|  (____  /__|   |__|           |____|__ \___|__/\____/|_____/   \n");

  xBinarySemaphore = xSemaphoreCreateBinary();

  motor_msg_Queue = xQueueCreate(10, sizeof(struct _knob_message *));
  motor_rcv_Queue = xQueueCreate(10, sizeof(struct _knob_message *));

  xTaskCreatePinnedToCore(
      Task_foc, "Task_foc", 4096, NULL, 2, &Task_foc_Handle, ESP32_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      Task_lvgl, "Task_lvgl", 4096, NULL, 3, &Task_lvgl_Handle, LVGL_RUNNING_CORE);

  interface_task.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
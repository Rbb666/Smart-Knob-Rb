#include <main.h>
#include <Hal/motor.h>
#include <display.h>
#include "interface_task.h"
#include "Hal/Ble_Keyboard.h"
#include "Hal/Wifi_Interface.h"

#define LVGL_RUNNING_CORE 1
#define ESP32_RUNNING_CORE 0

TaskHandle_t Task_foc_Handle;  // foc task
TaskHandle_t Task_lvgl_Handle; // lvgl task

QueueHandle_t motor_msg_Queue = NULL; // lvgl 接收消息队列
QueueHandle_t motor_rcv_Queue = NULL; // motor 接收消息队列

SemaphoreHandle_t xBinarySemaphore = NULL; // 二值信号量

_knob_message LVGL_MSG;
_knob_message MOTOR_MSG;

Wifi_Task wifi_task = Wifi_Task(ESP32_RUNNING_CORE);
InterfaceTask interface_task = InterfaceTask(ESP32_RUNNING_CORE);
Ble_Interface ble_dev;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Serial.printf("    _________                      __             ____  __.            ___.      \n");
  // Serial.printf("  /   _____/ _____ _____ ________/  |_          |    |/ _| ____   _____|_ |__    \n");
  // Serial.printf("  \_____  \ /     \__   \_  __ \   __\  ______  |      <  /    \ /  _ \| __  \   \n");
  // Serial.printf("  /        \  Y Y  \/ __ \|  | \/|  |   /_____/ |    |  \|   |  (  <_> ) \_\  \  \n");
  // Serial.printf(" /_________/__|_|  (____  /__|   |__|           |____|__ \___|__/\____/|_____/   \n");

  Serial.printf("Total heap: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

  xBinarySemaphore = xSemaphoreCreateBinary();

  motor_msg_Queue = xQueueCreate(10, sizeof(struct _knob_message *));
  motor_rcv_Queue = xQueueCreate(10, sizeof(struct _knob_message *));

  xTaskCreatePinnedToCore(
      Task_foc, "Task_foc", 2 * 1024, NULL, 5, &Task_foc_Handle, ESP32_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      Task_lvgl, "Task_lvgl", 5 * 1024, NULL, 3, &Task_lvgl_Handle, LVGL_RUNNING_CORE);

  wifi_task.begin();
  interface_task.begin();

  ble_dev.begin();
}

void loop() {}
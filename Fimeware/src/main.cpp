#include <main.h>
#include <Hal/motor.h>
#include <display.h>
#include "interface_task.h"
#include "Hal/Ble_Keyboard.h"
#include "Hal/Wifi_Interface.h"

#include <SPIFFS.h>

#define LVGL_RUNNING_CORE 1
#define ESP32_RUNNING_CORE 0

TaskHandle_t Task_foc_Handle;  // foc task
TaskHandle_t Task_lvgl_Handle; // lvgl task

QueueHandle_t motor_msg_Queue = NULL;
QueueHandle_t motor_rcv_Queue = NULL;

SemaphoreHandle_t xBinarySemaphore = NULL; // 二值信号量

_knob_message LVGL_MSG;
_knob_message MOTOR_MSG;

Ble_Interface ble_dev;
Wifi_Task wifi_task = Wifi_Task(ESP32_RUNNING_CORE);
InterfaceTask interface_task = InterfaceTask(ESP32_RUNNING_CORE);

void setup()
{
  Serial.begin(115200);

  const char ChappieCore_logo[]{
      "   _____                              _              _  __                  _ \n"
      "  / ____|                            | |            | |/ /                 | |\n"
      " | (___    _ __ ___     __ _   _ __  | |_   ______  | ' /   _ __     ___   | |__\n"
      "  \\___ \\  | '_ ` _ \\   / _` | | '__| | __| |______| |  <   | '_ \\   / _ \\  | '_ \\\n"
      "  ____) | | | | | | | | (_| | | |    | |_           | . \\  | | | | | (_) | | |_) |\n"
      " |_____/  |_| |_| |_|  \\__,_| |_|     \\__|          |_|\\_\\ |_| |_|  \\___/  |_.__/\n"};

  /* Print out */
  Serial.println(ChappieCore_logo);

  while (!SPIFFS.begin(true))
  {
    delay(500);
    Serial.print("...");
  }

  xBinarySemaphore = xSemaphoreCreateBinary();
  motor_msg_Queue = xQueueCreate(10, sizeof(struct _knob_message *));
  motor_rcv_Queue = xQueueCreate(10, sizeof(struct _knob_message *));

  xTaskCreatePinnedToCore(
      Task_foc, "Task_foc", 2 * 1024, NULL, 2, &Task_foc_Handle, ESP32_RUNNING_CORE);

  xTaskCreatePinnedToCore(
      Task_lvgl, "Task_lvgl", 4 * 1024, NULL, 2, &Task_lvgl_Handle, LVGL_RUNNING_CORE);

  wifi_task.begin();

  interface_task.begin();

  ble_dev.begin();
}

void loop() { yield(); }
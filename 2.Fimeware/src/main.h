#pragma once

#include <Arduino.h>

struct _knob_message
{
    int ucMessageID;
    char ucData[100];
};

extern _knob_message LVGL_MSG;
extern _knob_message MOTOR_MSG;

extern SemaphoreHandle_t xBinarySemaphore;

extern QueueHandle_t motor_msg_Queue; // lvgl 接收消息队列
extern QueueHandle_t motor_rcv_Queue; // motor 接收消息队列
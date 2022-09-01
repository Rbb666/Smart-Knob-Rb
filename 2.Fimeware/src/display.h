#pragma once
#include <Arduino.h>

typedef enum
{
    CHECKOUT_PAGE,
    BUTTON_CLICK

} DISPLAY_STATUS_E;

void Task_lvgl(void *pvParameters);
void update_page_status(int page_status);

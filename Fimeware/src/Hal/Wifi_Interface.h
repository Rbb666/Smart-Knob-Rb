#pragma once
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>

#include "task.h"

class Wifi_Task : public Task<Wifi_Task>
{
    friend class Task<Wifi_Task>;

public:
    Wifi_Task(const uint8_t task_core);

    ~Wifi_Task();

    void http_get_wether(void);

protected:
    void run();

private:
    const char *ssid = "501";
    const char *password = "Benxiaohai123";
};
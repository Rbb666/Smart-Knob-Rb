#pragma once

#include "Arduino.h"

struct SysUtilConfig
{
    String ssid_0;
    String password_0;
    String ssid_1;
    String password_1;
    String ssid_2;
    String password_2;
};

boolean doDelayMillisTime(unsigned long interval,
                          unsigned long *previousMillis);

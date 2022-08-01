#ifndef COMMON_H
#define COMMON_H

#include "Arduino.h"
#include "HAL/flash_fs.h"

extern FlashFS g_flashCfg;

struct SysUtilConfig
{
    String ssid_0;
    String password_0;
    String ssid_1;
    String password_1;
    String ssid_2;
    String password_2;

    // boolean auto_conn_wifi;
};

boolean doDelayMillisTime(unsigned long interval,
                          unsigned long *previousMillis);
#endif

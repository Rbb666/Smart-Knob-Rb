#include "Common.h"

FlashFS g_flashCfg;

boolean doDelayMillisTime(unsigned long interval, unsigned long *previousMillis)
{
    unsigned long currentMillis = millis();
    if (currentMillis - *previousMillis >= interval)
    {
        *previousMillis = currentMillis;
        return true;
    }
    return false;
}

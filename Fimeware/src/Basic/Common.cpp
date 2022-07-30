#include "Common.h"
#include <nvs_flash.h>

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

uint8_t NvsDataToFlash_Init(void)
{
    /* 初始化非易失性存储库 (NVS) */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        /* NVS分区被截断，需要删除,然后重新初始化NVS */
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    Serial.println("NvsDataToFlash_Init success!!");
    return ESP_OK;
}

#pragma once
#include <Arduino.h>

#include "Basic/Common.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>

#include "task.h"

class Wifi_Task : public Task<Wifi_Task>
{
    friend class Task<Wifi_Task>;

    enum WIFI_BASIC_STATE
    {
        WIFI_IDLE,
        WIFI_READY
    };

    enum WIFI_CONN_STATE
    {
        CONN_SUCCESS,
        CONN_ERROR
    };

    enum APP_MESSAGE_TYPE
    {
        APP_MESSAGE_IDLE = 0,
        APP_MESSAGE_WIFI_CONN,    // 开启连接
        APP_MESSAGE_WIFI_AP,      // 开启 AP 事件
        APP_MESSAGE_WIFI_ALIVE,   // wifi 开关的心跳维持
        APP_MESSAGE_WIFI_DISCONN, // 连接断开
        APP_MESSAGE_UPDATE_TIME,
        APP_MESSAGE_MQTT_DATA, // MQTT 客户端收到消息
        APP_MESSAGE_GET_PARAM, // 获取参数
        APP_MESSAGE_SET_PARAM, // 设置参数

        APP_MESSAGE_NONE
    };

    struct wifi_message
    {
        uint8_t wifi_state_type;
    };

public:
    Wifi_Task(const uint8_t task_core);

    ~Wifi_Task();

    void start(void);
    void update_wifi_status(uint8_t wifi_state);

    void search_wifi(void);
    boolean start_conn_wifi(const char *ssid, const char *password);
    boolean end_conn_wifi(void);
    boolean open_ap(const char *ap_ssid, const char *ap_password = NULL);
    boolean close_wifi(void);

    void http_get_wether(void);

protected:
    void run();

private:
    const char *ssid = "501";
    const char *password = "Benxiaohai123";

    const char *AP_SSID = "Smart-Knob"; // 热点名
    const char *HOST_NAME = "eps32";    // 主机名

    unsigned long wifi_conn_millis;
    boolean wifi_status;              // wifi状态 true开启 false关闭
    unsigned long m_preWifiReqMillis; // 保存上一回请求的时间戳
    SysUtilConfig sys_cfg;            // 持久化保存参数

    wifi_message WIFI_MSG;
    QueueHandle_t wifi_rcv_Queue;

    boolean wifi_event(uint8_t type);
};
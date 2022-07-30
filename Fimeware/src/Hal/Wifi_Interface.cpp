#include "Wifi_Interface.h"

IPAddress local_ip(192, 168, 4, 2); // Set your server's fixed IP address here
IPAddress gateway(192, 168, 4, 2);  // Set your network Gateway usually your Router base address
IPAddress subnet(255, 255, 255, 0); // Set your network sub-network mask here
IPAddress dns(192, 168, 4, 1);      // Set your network DNS usually your Router base address

Wifi_Task::Wifi_Task(const uint8_t task_core) : Task("Wifi", 2 * 1024, 1, task_core) {}

Wifi_Task::~Wifi_Task() {}

WebServer server(80);

void Wifi_Task::run()
{
    Serial.println("Starting Wifi work!");

    wifi_conn_millis = 0;

    wifi_rcv_Queue = xQueueCreate(10, sizeof(wifi_message *));

    for (;;)
    {
        wifi_message *message;
        if (xQueueReceive(wifi_rcv_Queue, &(message), (TickType_t)0))
        {
           wifi_event(message->wifi_state_type);
        }

        vTaskDelay(100);
    }
}

void Wifi_Task::start(void)
{
    update_wifi_status(APP_MESSAGE_WIFI_AP);
}

void Wifi_Task::update_wifi_status(uint8_t wifi_state)
{
    struct wifi_message *send_message;
    send_message = &WIFI_MSG;
    send_message->wifi_state_type = wifi_state;
    xQueueSend(wifi_rcv_Queue, &send_message, (TickType_t)0u);
    Serial.printf("send_message to wifi Queue --->(%d)\n", send_message->wifi_state_type);
}

void Wifi_Task::search_wifi(void)
{
    Serial.println("scan start");
    int wifi_num = WiFi.scanNetworks();
    Serial.println("scan done");
    if (0 == wifi_num)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(wifi_num);
        Serial.println("networks found");
        for (int cnt = 0; cnt < wifi_num; ++cnt)
        {
            Serial.print(cnt + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(cnt));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(cnt));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(cnt) == WIFI_AUTH_OPEN) ? " " : "*");
        }
    }
}

boolean Wifi_Task::start_conn_wifi(const char *ssid, const char *password)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(F("\nWiFi is OK.\n"));
        return false;
    }
    Serial.println("");
    Serial.print(F("Connecting: "));
    Serial.print(ssid);
    Serial.print(F(" @ "));
    Serial.println(password);

    // 设置STA模式
    WiFi.enableSTA(true);
    // 修改主机名
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(ssid, password);
    wifi_conn_millis = millis();

    return true;
}

boolean Wifi_Task::end_conn_wifi(void)
{
    if (WL_CONNECTED != WiFi.status())
    {
        if (doDelayMillisTime(5000, &wifi_conn_millis))
        {
            Serial.println(F("\nWiFi connect error.\n"));
        }
        return -1;
    }

    if (doDelayMillisTime(10000, &wifi_conn_millis))
    {
        Serial.println(F("\nWiFi connected"));
        Serial.print(F("IP address: "));
        Serial.println(WiFi.localIP());
    }
    return 0;
}

boolean Wifi_Task::open_ap(const char *ap_ssid, const char *ap_password)
{
    WiFi.enableAP(true); // 配置为AP模式
    // 修改主机名
    WiFi.setHostname("esp32");
    boolean result = WiFi.softAP(ap_ssid, ap_password); // 开启热点
    if (result)
    {
        WiFi.softAPConfig(local_ip, gateway, subnet);
        IPAddress myIP = WiFi.softAPIP();

        //打印相关信息
        Serial.print(F("\nSoft-AP IP address = "));
        Serial.println(myIP);
        Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
        Serial.println(F("WiFiAP Success"));
    }
    else
    {
        //开启热点失败
        Serial.println(F("WiFiAP Failed"));
        return false;
    }
    return true;
}

boolean Wifi_Task::close_wifi(void)
{
    if (!WiFi.disconnect())
    {
        return false;
    }
    WiFi.enableSTA(false);
    WiFi.enableAP(false);
    WiFi.mode(WIFI_MODE_NULL);
    Serial.println(F("WiFi disconnect"));
    return true;
}

/**
 *  wifi事件的处理
 *  事件处理成功返回true 否则false
 * */
boolean Wifi_Task::wifi_event(uint8_t type)
{
    switch (type)
    {
    case APP_MESSAGE_IDLE:
        break;
    case APP_MESSAGE_WIFI_CONN:
    {
        if (WIFI_IDLE == wifi_status)
        {
            start_conn_wifi(sys_cfg.ssid_0.c_str(), sys_cfg.password_0.c_str());
            wifi_status = WIFI_READY;
        }

        m_preWifiReqMillis = millis();
        if ((WiFi.getMode() & WIFI_MODE_STA) == WIFI_MODE_STA && CONN_SUCCESS != end_conn_wifi())
        {
            // 在STA模式下 并且还没连接上wifi
            return false;
        }
    }
    break;
    case APP_MESSAGE_WIFI_AP:
    {
        m_preWifiReqMillis = millis();
        if (true != open_ap(AP_SSID))
        {
            wifi_status = WIFI_READY;
            return false;
        }
    }
    break;
    case APP_MESSAGE_WIFI_ALIVE:
    {
        // wifi 心跳
        wifi_status = WIFI_READY;
        // 更新请求
        m_preWifiReqMillis = millis();
    }
    break;
    case APP_MESSAGE_WIFI_DISCONN:
    {
        close_wifi();
        wifi_status = WIFI_IDLE;
    }
    break;
    default:
        break;
    }
    return true;
}

void Wifi_Task::http_get_wether()
{
}
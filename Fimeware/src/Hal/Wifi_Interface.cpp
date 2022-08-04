#include "Wifi_Interface.h"
#include "Page_html.h"
#include "HAL/flash_fs.h"

#include <ElegantOTA.h>

IPAddress local_ip(192, 168, 4, 2); // Set your server's fixed IP address here
IPAddress gateway(192, 168, 4, 2);  // Set your network Gateway usually your Router base address
IPAddress subnet(255, 255, 255, 0); // Set your network sub-network mask here
IPAddress dns(192, 168, 4, 1);      // Set your network DNS usually your Router base address

WebServer server(80);

Wifi_Task::Wifi_Task(const uint8_t task_core) : Task("Wifi", 3 * 1024, 1, task_core) {}

Wifi_Task::~Wifi_Task(void) {}

static void wifi_timeout_callback(TimerHandle_t pxTimer);

void Wifi_Task::run(void)
{
    // 读取参数
    read_config(&sys_cfg);
    wifi_state_config();

    // 创建wifi超时定时器-1min
    wifi_timeout_tmr = xTimerCreate("wifi_Timer",
                                    (1000 * 60),
                                    pdTRUE,
                                    (void *)0,
                                    wifi_timeout_callback);

    wifi_conn_millis = 0;
    Serial.println("Starting Wifi work!");

    for (;;)
    {
        FSM_Task();
        vTaskDelay(100);
    }
}

void Wifi_Task::read_config(SysUtilConfig *cfg)
{
#define CONFIG_PATH "/sys.cfg"

    char info[128] = {0};
    uint16_t size = g_flashCfg.readFile(CONFIG_PATH, (uint8_t *)info);
    info[size] = 0;
    if (size == 0)
    { // 默认值
        Serial.println("No Data in SPI_FS!");
    }
    else
    {
        // 解析数据
        char *param[12] = {0};
        analyseParam(info, 12, param);
        Serial.println("");
        Serial.println("");
        cfg->ssid_0 = param[0];
        cfg->password_0 = param[1];
        cfg->ssid_1 = param[2];
        cfg->password_1 = param[3];
        cfg->ssid_2 = param[4];
        cfg->password_2 = param[5];

        for (int i = 0; i < 6; i++)
            Serial.printf("SPI_FS Data:%s\n", param[i]);
    }
}

void Wifi_Task::wifi_state_config(void)
{
    if (sys_cfg.ssid_0 != "")
    {
        wifi_status = WIFI_STA_IDLE;
        task_state = TASK_STA_REQUEST;
    }
    else
    {
        wifi_status = WIFI_AP_IDLE;
        task_state = TASK_AP_REQUEST;
    }
}

void Wifi_Task::write_config(SysUtilConfig *cfg)
{
#define CONFIG_PATH "/sys.cfg"

    char tmp[25];
    //（持久化）
    String w_data;
    w_data = w_data + cfg->ssid_0 + "\n";
    w_data = w_data + cfg->password_0 + "\n";
    w_data = w_data + cfg->ssid_1 + "\n";
    w_data = w_data + cfg->password_1 + "\n";
    w_data = w_data + cfg->ssid_2 + "\n";
    w_data = w_data + cfg->password_2 + "\n";

    g_flashCfg.writeFile(CONFIG_PATH, w_data.c_str());
    Serial.printf("Write config to SPIFS...\n");
}

static void wifi_timeout_callback(TimerHandle_t pxTimer)
{
    Serial.println("Close WebServer Service!!");
    server.close();
    xTimerStop(pxTimer, 0);
}

// wifi 有限状态机
void Wifi_Task::FSM_Task(void)
{
    switch (task_state)
    {
    case TASK_IDLE:
    {
        server.handleClient();
    }
    break;
    case TASK_AP_REQUEST:
    {
        wifi_status = WIFI_AP_IDLE;

        boolean wifi_state = wifi_event(APP_MESSAGE_WIFI_AP);
        if (wifi_state)
            task_state = TASK_AP_CONNECTED;
    }
    break;
    case TASK_AP_CONNECTED:
    {
        Serial.println("Start AP WebServer");

        server.on("/", HTTP_GET, [&]()
                  { server.send_P(200, "text/html", (const char *)index_html, sizeof(index_html)); });
        server.on("/config", [&]()
                  {
                String name = server.arg("name").c_str();
                String password = server.arg("password").c_str();
                Serial.printf("[AP REC] wifi name:%s pass:%s\n", name, password);

                // 暂存web转发的信息
                sys_cfg.ssid_0 = name;
                sys_cfg.password_0 = password;

                // 切换到STA模式尝试连接
                task_state = TASK_STA_REQUEST;
                connect_mode = AP_TO_CONNECT_MODE;
                Serial.printf("Ready to connect[%s]\n", name); });

        server.begin();
        ElegantOTA.begin(&server);
        task_state = TASK_IDLE;
    }
    break;
    case TASK_STA_REQUEST:
    {
        wifi_status = WIFI_STA_IDLE; // 设置wifi为idel模式

        boolean wifi_state = wifi_event(APP_MESSAGE_WIFI_STA);
        if (wifi_state)
            task_state = TASK_STA_CONNECTED;
        else
            task_state = TASK_DISCONNECT;
    }
    break;
    case TASK_STA_CONNECTED:
    {
        Serial.println("Start STA WebServer");

        server.on("/", HTTP_GET, [&]()
                  { server.send_P(200, "text/html", (const char *)index_html, sizeof(index_html)); });
        server.on("/config", [&]()
                  {
                String name = server.arg("name").c_str();
                String password = server.arg("password").c_str();
                Serial.printf("[STA REC] wifi name:%s pass:%s\n", name, password); });

        server.begin();
        ElegantOTA.begin(&server);

        // AP 配网成功后保存参数
        if (connect_mode == AP_TO_CONNECT_MODE)
            task_state = TASK_NET_CONNECTED;
        else
            task_state = TASK_IDLE;

        // 开启超时定时器
        xTimerStart(wifi_timeout_tmr, 0);
    }
    break;
    case TASK_DISCONNECT:
    {
        static uint8_t recon_count = 0;
        recon_count++;
        if (recon_count < 15)
        {
            Serial.printf("wifi connect fail, ready to reconnect[%d]...\n\n", recon_count);
            vTaskDelay(500);
            // 重连成功
            if (end_conn_wifi())
                task_state = TASK_STA_CONNECTED;
        }
        else
        {
            wifi_event(APP_MESSAGE_WIFI_DISCONN); // close wifi
            task_state = TASK_TRASH;
            recon_count = 0;
        }
    }
    break;
    // AP 配网成功模式
    case TASK_NET_CONNECTED:
    {
        // 保存配置信息
        write_config(&sys_cfg);
        task_state = TASK_IDLE;
    }
    break;
    case TASK_TRASH:
        break;
    default:
        break;
    }
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
        Serial.println(F("\nWiFi is Connected.\n"));
        return false;
    }
    Serial.println("");
    Serial.print(F("Connecting: "));
    Serial.print(ssid);
    Serial.print(F(" @ "));
    Serial.println(password);

    // 设置STA模式
    WiFi.mode(WIFI_MODE_STA);
    // 修改主机名
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(ssid, password);
    WiFi.setAutoReconnect(true);
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
        return false;
    }

    Serial.println(F("\nWiFi connected"));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());

    return true;
}

boolean Wifi_Task::open_ap(const char *ap_ssid, const char *ap_password)
{
    WiFi.mode(WIFI_AP_STA); // 配置为AP+STA模式
    // 修改主机名
    WiFi.setHostname("esp32");
    boolean result = WiFi.softAP(ap_ssid, ap_password); // 开启热点
    if (result)
    {
        WiFi.softAPConfig(local_ip, gateway, subnet);
        IPAddress myIP = WiFi.softAPIP();

        //打印相关信息
        Serial.print(F("Soft-AP IP address = \n"));
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
boolean Wifi_Task::wifi_event(APP_MESSAGE_TYPE type)
{
    switch (type)
    {
    case APP_MESSAGE_IDLE:
        break;
    case APP_MESSAGE_WIFI_STA:
    {
        if (wifi_status == WIFI_STA_IDLE)
        {
            start_conn_wifi(sys_cfg.ssid_0.c_str(), sys_cfg.password_0.c_str());
            wifi_status = WIFI_STA_READY;
        }

        m_preWifiReqMillis = millis();
        if (((WiFi.getMode() & WIFI_MODE_STA) == WIFI_MODE_STA) && (CONN_ERROR != end_conn_wifi()))
        {
            // 在STA模式下 并且还没连接上wifi
            return false;
        }
    }
    break;
    case APP_MESSAGE_WIFI_AP:
    {
        boolean ap_state;
        if (wifi_status == WIFI_AP_IDLE)
        {
            ap_state = open_ap(AP_SSID);
            wifi_status = WIFI_AP_READY;
        }

        m_preWifiReqMillis = millis();
    }
    break;
    case APP_MESSAGE_WIFI_ALIVE:
    {
        // wifi 心跳
        wifi_status = WIFI_STA_IDLE;
        // 更新请求
        m_preWifiReqMillis = millis();
    }
    break;
    case APP_MESSAGE_WIFI_DISCONN:
    {
        close_wifi();
        wifi_status = WIFI_STA_IDLE;
    }
    break;
    default:
        break;
    }
    return true;
}

void Wifi_Task::http_get_wether(void)
{
}
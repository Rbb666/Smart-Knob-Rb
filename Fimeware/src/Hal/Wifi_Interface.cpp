#include "Wifi_Interface.h"

Wifi_Task::Wifi_Task(const uint8_t task_core) : Task("Wifi", 1024, 1, task_core) {}

Wifi_Task::~Wifi_Task() {}

WebServer server(80);

void Wifi_Task::run()
{
    Serial.println("Starting Wifi work!");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    ElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();

    for (;;)
    {
        server.handleClient();
        vTaskDelay(100);
    }
}

void Wifi_Task::http_get_wether()
{
}
#include <Arduino.h>
#include "WifiSetup.h"
#include "ESP32Ping.h" // Include the ESP32Ping library

WifiSetup wifi(g_ssid, g_password);

void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
    Serial.begin(115200);

    // Setup WiFi
    wifi.connect();
}

void loop()
{
    wifi.loop();
}

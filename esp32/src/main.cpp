// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/

#include <Arduino.h>
#include "MPU6050.h"
#include "WifiSetup.h"
#include "ESP32Ping.h" // Include the ESP32Ping library

WifiSetup wifi(g_ssid, g_password);
MPU6050 mpu;
  
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

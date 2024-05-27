// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/

#include <Arduino.h>
#include "PidController.h"
#include "WifiSetup.h"

WifiSetup wifi(g_ssid, g_password);
// PidController is a static class, so we don't need to create an instance of it

void pidLoop(void *pvParameters);

void setup()
{
    Serial.begin(115200);

    // Setup modules
    wifi.connect();
    // PidController::setup();

    // Setup the pidLoop
    // xTaskCreatePinnedToCore(
    //     pidLoop,   /* Task function. */
    //     "pidLoop", /* name of task. */
    //     10000,     /* Stack size of task = 40 KB */
    //     NULL,      /* parameter of the task */
    //     1,         /* priority of the task */
    //     NULL,      /* Task handle to keep track of created task */
    //     0);        /* pin task to core 0 */
}

void loop()
{
    wifi.loop();
}

void pidLoop(void *pvParameters)
{
    while (true)
    {
        // PidController::loop();
    }
}

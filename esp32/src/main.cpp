// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/

#include <Arduino.h>
#include "WifiSetup.h"
#include "SerialLoop.h"
#include "PidController.h"

WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);
// WifiSetup wifi(i_ssid, i_email, i_password, MQTT_SERVER, MQTT_PORT);
// PidController is a static class, so we don't need to create an instance of it

// The loop interval in milliseconds (should be ULONG_MAX for normal operation)
unsigned long timeout = ULONG_MAX;

// Declare variables for RunID and BotID
int BotID, RunID;

void pidLoop(void *pvParameters);
void serialLoop(void *pvParameters);

void setup()
{
    Serial.begin(115200);

    // Setup modules
    wifi.connect(timeout);

    if (PidController::setup(wifi, timeout))
    {
        // Setup the pidLoop
        xTaskCreatePinnedToCore(
            pidLoop,   /* Task function. */
            "pidLoop", /* name of task. */
            10000,     /* Stack size of task = 40 KB */
            NULL,      /* parameter of the task */
            1,         /* priority of the task */
            NULL,      /* Task handle to keep track of created task */
            1);        /* pin task to core 1 */
    }

    // Setup the serialLoop0
    xTaskCreatePinnedToCore(
        serialLoop,   /* Task function. */
        "serialLoop", /* name of task. */
        10000,        /* Stack size of task = 40 KB */
        NULL,         /* parameter of the task */
        1,            /* priority of the task */
        NULL,         /* Task handle to keep track of created task */
        1);           /* pin task to core 1 */
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
        PidController::stabilizedLoop();
    }
}

void serialLoop(void *pvParameters)
{
    while (true)
    {
        serialLoop();
    }
}

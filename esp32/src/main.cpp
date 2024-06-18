/*
ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/

The stack size of xTaskCreatePinnedToCore() is in bytes not words (differs from
vanilla FreeRTOS). Source (also read the documentation):
https://www.ojisanseiuchi.com/2024/02/20/freertos-stack-size-on-esp32-words-or-bytes/
*/

#include <Arduino.h>
#include "BatteryModule.h"
#include "WifiSetup.h"
#include "BatteryModule.h"
#include "SerialLoop.h"
#include "PidController.h"

// WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);
WifiSetup wifi(i_ssid, i_email, i_password, MQTT_SERVER, MQTT_PORT);
// PidController is a static class, so we don't need to create an instance of it

// The loop interval in milliseconds (should be ULONG_MAX for normal operation)
unsigned long timeout = ULONG_MAX;

// Declare variables for RunID and BotID
int BotID, RunID;

void wifiLoop(void *pvParameters);
void pidLoop(void *pvParameters);
void serialLoop(void *pvParameters);
void batteryLoop(void *pvParameters);

void setup()
{
    Serial.begin(115200);

    // Setup modules
    wifi.connect(timeout);
    BatteryModule::setup();

    // Setup the wifiLoop
    // Important: wifiLoop must run on core 0 due to ESP32 restrictions
    xTaskCreatePinnedToCore(
        wifiLoop,   /* Task function. */
        "wifiLoop", /* name of task. */
        10000,      /* Stack size of task = 10 KB */
        NULL,       /* parameter of the task */
        1,          /* priority of the task */
        NULL,       /* Task handle to keep track of created task */
        0);         /* pin task to core 0 */

    delay(100);

    if (PidController::setup(wifi, timeout))
    {
        // Setup the pidLoop
        xTaskCreatePinnedToCore(
            pidLoop,   /* Task function. */
            "pidLoop", /* name of task. */
            30000,     /* Stack size of task = 30 KB */
            NULL,      /* parameter of the task */
            2,         /* priority of the task */
            NULL,      /* Task handle to keep track of created task */
            1);        /* pin task to core 1 */
    }
    else
    {
        wifi.println("Failed to setup PidController");
    }

    // Setup the serialLoop
    xTaskCreatePinnedToCore(
        serialLoop,      /* Task function. */
        "serialLoop",    /* name of task. */
        10000,           /* Stack size of task = 10 KB */
        NULL,            /* parameter of the task */
        1,               /* priority of the task */
        NULL,            /* Task handle to keep track of created task */
        tskNO_AFFINITY); /* core 0/1 */

    // Setup the batteryLoop
    xTaskCreatePinnedToCore(
        batteryLoop,     /* Task function. */
        "batteryLoop",   /* name of task. */
        15000,           /* Stack size of task = 15 KB */
        NULL,            /* parameter of the task */
        1,               /* priority of the task */
        NULL,            /* Task handle to keep track of created task */
        tskNO_AFFINITY); /* core 0/1 */
}

// loop() is the only task not guaranteed to run in RTOS
void loop()
{
}

void wifiLoop(void *pvParameters)
{
    while (true)
    {
        wifi.loop();
    }
}

void pidLoop(void *pvParameters)
{
    while (true)
    {
        // PidController::loop();
        PidController::stabilizedLoop();

        // Delay to allow other tasks to run
        yield();
    }
}

void serialLoop(void *pvParameters)
{
    while (true)
    {
        SerialLoop::loop();
    }
}

void batteryLoop(void *pvParameters)
{
    while (true)
    {
        BatteryModule::loop();
    }
}

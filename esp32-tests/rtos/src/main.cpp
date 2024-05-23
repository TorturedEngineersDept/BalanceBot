/**
 * This blinks the LED using a timer whilst schedules 2 tasks on the same core.
 * Provides a simple example for RTOS on the ESP32.
 */

#include <Arduino.h>
#include <TimerInterrupt_Generic.h>

#define INTERNAL_LED 2

// The ESP32 has 4 times. Use the first one.
ESP32Timer ITimer(0);

void loop2(void *pvParameters);

bool timerHandler(void *timerNo)
{
    static bool ledState = false;

    ledState = !ledState;
    digitalWrite(INTERNAL_LED, ledState);

    return true;
}

void setup()
{
    Serial.begin(115200);

    if (!ITimer.attachInterruptInterval(1000000, timerHandler))
    {
        Serial.println("Failed to attach interrupt handler");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("Timer attached");

    pinMode(INTERNAL_LED, OUTPUT);
    digitalWrite(INTERNAL_LED, LOW);

    // Pin to core 0 to reserve core 1 for WiFi tasks
    xTaskCreatePinnedToCore(
        loop2,   /* Function to implement the task */
        "loop2", /* Name of the task */
        1000,    /* Stack size in words */
        NULL,    /* Task input parameter */
        0,       /* Priority of the task */
        NULL,    /* Task handle. */
        0);      /* Core where the task should run */
}

void loop()
{
    Serial.println("Hello World from loop()!");
    delay(2000);
}

void loop2(void *pvParameters)
{
    while (1)
    {
        Serial.println("Hello World from loop2()!");
        delay(4000);
    }
}

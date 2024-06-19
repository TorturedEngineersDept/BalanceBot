#pragma once

#include <cstdint>
#include "KeyDirection.h"

/**
 * Takes an 3-bit message and decodes to direction and speed
 * The first 3 bits are the direction
 */
void decode(uint8_t message, KeyDirection &direction)
{
    direction = static_cast<KeyDirection>(message & 0b00000111);
}

#ifndef NATIVE

#include <Arduino.h>
#include "PidController.h"

namespace SerialLoop
{
    /**
     * Allows the Raspberry Pi to communicate with the ESP32 over serial.
     * Decodes as a 8-bit ASCII
     */
    void loop()
    {
        // Discard all buffer data
        while (Serial.available() > 1)
        {
            Serial.read();
        }

        if (Serial.available() < 1)
        {
            return;
        }

        char message = Serial.read();

        KeyDirection direction;
        decode(message, direction);

        switch (direction)
        {
        case KeyDirection::FORWARD:
            Serial.println("[rpi] FORWARD");
            break;
        case KeyDirection::BACKWARD:
            Serial.println("[rpi] BACKWARD");
            break;
        case KeyDirection::LEFT:
            Serial.println("[rpi] LEFT");
            break;
        case KeyDirection::RIGHT:
            Serial.println("[rpi] RIGHT");
            break;
        case KeyDirection::STOP:
            Serial.println("[rpi] STOP");
            break;
        }

        if (xSemaphoreTake(PidController::controlMutex, (TickType_t)0) == pdTRUE)
        {
            PidController::setDirection(PidDirection(100, direction));
            Serial.println("Speed: 100, key_dir: " + String(direction));
            xSemaphoreGive(PidController::controlMutex);
        }
        else
        {
            Serial.println("Enter /a to give control back to the Raspberry Pi");
        }

        // Yield to other tasks
        delay(10);
    }
}

#endif

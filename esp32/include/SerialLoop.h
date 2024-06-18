#pragma once

#include <cstdint>
#include "KeyDirection.h"

/**
 * Takes an 8-bit message and decodes to direction and speed
 * The first 3 bits are the direction
 * The last 5 bits are the speed
 */
void decode(uint8_t message, KeyDirection &direction, uint8_t &speed)
{
    direction = static_cast<KeyDirection>(message >> 5);
    speed = message & 0b00011111;
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
        Serial.println("SERIAL");
        // Discard all buffer data
        while (Serial.available() > 1)
        {
            Serial.read();
        }

        char message = Serial.read();

        KeyDirection direction;
        uint8_t speed;
        decode(message, direction, speed);

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
            PidController::setDirection(PidDirection(speed, direction));
            xSemaphoreGive(PidController::controlMutex);
        }

        // Yield to other tasks
        delay(10);
    }
}

#endif

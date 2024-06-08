#pragma once

#include <cstdint>

enum class Direction
{
    RIGHT = 0,
    FORWARD = 1,
    LEFT = 2,
    BACKWARD = 3
};

#ifndef NATIVE

#include <Arduino.h>

/**
 * Allows the Raspberry Pi to communicate with the ESP32 over serial.
 * Decodes as a 8-bit ASCII
 */
void serialLoop()
{
    while (true)
    {
        // Discard all buffer data
        while (Serial.available() > 1)
        {
            Serial.read();
        }

        char message = Serial.read();

        Direction direction;
        uint8_t speed;
        decode(message, direction, speed);

        switch (direction)
        {
        case Direction::RIGHT:
            break;
        case Direction::FORWARD:
            break;
        case Direction::LEFT:
            break;
        case Direction::BACKWARD:
            break;
        }
    }
}

#endif

/**
 * Takes an 8-bit message and decodes to direction and speed
 * The first 2 bits are the direction
 * The last 6 bits are the speed
 */
void decode(uint8_t message, Direction &direction, uint8_t &speed)
{
    direction = static_cast<Direction>(message >> 6);
    speed = message & 0b00111111;
}

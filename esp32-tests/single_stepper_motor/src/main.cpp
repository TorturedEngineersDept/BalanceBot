// Single Stepper Motor Test
// Author: Sanjit Raman
// Date: 21 May 2024
// Version: 1.0.0
// Description:
//    This code is meant to work with an A4988 motor driver connected to the
//    microcontroller with the pinouts as described above. The expected result:
//        - the stepper motor spins FORWARDS for 1 second
//        - waits for 1 second
//        - the stepper motor spins BACKWARDS for 1 second
//        - at the end the motor should return to the same position it started
//        at.

#include <Arduino.h>

// Define pin connections & motor's steps per revolution
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;

void setup()
{
    // Declare pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
}
void loop()
{
    // Set motor direction clockwise
    digitalWrite(dirPin, HIGH);

    // Spin motor slowly
    for (int x = 0; x < stepsPerRevolution; x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second

    // Set motor direction counterclockwise
    digitalWrite(dirPin, LOW);

    // Spin motor quickly
    for (int x = 0; x < stepsPerRevolution; x++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(1000);
    }
    delay(1000);  // Wait a second
}

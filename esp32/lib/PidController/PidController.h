#pragma once

#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include "MPU6050.h"
#include "Step.h"

// The Stepper pins
#define STEPPER1_DIR_PIN 16  // Arduino D9
#define STEPPER1_STEP_PIN 17 // Arduino D8
#define STEPPER2_DIR_PIN 4   // Arduino D11
#define STEPPER2_STEP_PIN 14 // Arduino D10
#define STEPPER_EN 15        // Arduino D12

// The ESP32 has 4 timers. Use the third one.
#define PID_TIMER_NO 3

// Diagnostic pin for oscilloscope
#define TOGGLE_PIN 32 // Arduino A4

class PidController
{
public:
    PidController() = delete;
    
    static void setup();
    static void loop();
    static bool timerHandler(void *args);

private:
    static MPU6050 mpu;
    static ESP32Timer ITimer;
    static Step step1;
    static Step step2;

    // Class constants
    static constexpr int PRINT_INTERVAL = 50;
    static constexpr double LOOP_INTERVAL = 5;
    static constexpr int STEPPER_INTERVAL_US = 10;
    static constexpr float MOTOR_ACCEL_RAD = 30.0;
};

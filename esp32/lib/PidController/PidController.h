#pragma once

#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include <freertos/semphr.h>
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

struct PidParams
{
    PidParams(float Kp, float Ki, float Kd, float setpoint)
        : Kp(Kp), Ki(Ki), Kd(Kd), setpoint(setpoint) {}

    float Kp;
    float Ki;
    float Kd;
    float setpoint;
};

struct PidDirection
{
    PidDirection(float speed, float angle)
        : speed(speed), angle(angle) {}

    float speed;
    float angle;
};

class PidController
{
public:
    PidController() = delete;

    static void setup();
    static void loop();

    /**
     * The callback function to set the PID parameters.
     */
    static void setParams(PidParams params);

    static PidParams getParams();

    /**
     * The callback function to set the direction of the motor.
     */
    static void setDirection(PidDirection direction);

    static PidDirection getDirection();

private:
    static bool timerHandler(void *args);

    static MPU6050 mpu;
    static ESP32Timer ITimer;
    static Step step1;
    static Step step2;
    static PidParams params;
    static PidDirection direction;
    static SemaphoreHandle_t paramsMutex;
    static SemaphoreHandle_t directionMutex;

    // Class constants
    static constexpr int PRINT_INTERVAL = 50;
    static constexpr double LOOP_INTERVAL = 5;
    static constexpr int STEPPER_INTERVAL_US = 10;
    static constexpr float MOTOR_ACCEL_RAD = 30.0;
};

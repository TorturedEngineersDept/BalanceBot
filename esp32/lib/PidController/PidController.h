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
    PidParams(float Ki_i, float Kp_i, float Kd_i, float setpoint_i, float Ki_o, float Kp_o, float Kd_o, float setpoint_o)
        : Kp_i(Kp_i), Ki_i(Ki_i), Kd_i(Kd_i), setpoint_i(setpoint_i), Kp_o(Kp_o), Ki_o(Ki_o), Kd_o(Kd_o), setpoint_o(setpoint_o) {}

    float Kp_i;
    float Ki_i;
    float Kd_i;
    float setpoint_i;
    float Kp_o;
    float Ki_o;
    float Kd_o;
    float setpoint_o;
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

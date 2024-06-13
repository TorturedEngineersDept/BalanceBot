#pragma once

#include <Arduino.h>
#include <freertos/semphr.h>
#include <TimerInterrupt_Generic.h>
#include "MPU6050.h"
#include "KeyDirection.h"
#include "Step.h"
#include "IWifi.h"

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
    PidParams(float ki_i, float kp_i, float kd_i, float tilt_setpoint,
              float ki_o, float kp_o, float kd_o, float velocity_setpoint, float rotation_setpoint)
        : kp_i(kp_i), ki_i(ki_i), kd_i(kd_i), tilt_setpoint(tilt_setpoint),
          kp_o(kp_o), ki_o(ki_o), kd_o(kd_o), velocity_setpoint(velocity_setpoint),
          rotation_setpoint(rotation_setpoint) {}

    float kp_i;
    float ki_i;
    float kd_i;
    float tilt_setpoint;
    float kp_o;
    float ki_o;
    float kd_o;
    float velocity_setpoint;
    float rotation_setpoint;
};

struct PidDirection
{
    PidDirection(float speed, KeyDirection key_dir)
        : speed(speed), key_dir(key_dir) {}

    float speed;
    KeyDirection key_dir;
};

class PidController
{
public:
    PidController() = delete;

    /**
     * Setup the PID controller.
     * @return true if the setup was successful, false otherwise.
     */
    static bool setup(IWifi &wifi, unsigned long timeout = ULONG_MAX);
    static void loop();

    /**
     * A temporary function to get a stabilized version of the robot moving
     * (for testing purposes only).
     */
    static void stabilizedLoop();

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

    // Public members
    static SemaphoreHandle_t controlMutex;

private:
    static bool timerHandler(void *args);

    /**
     * Controls tilt.
     */
    static void innerLoop();

    /**
     * Controls velocity.
     */
    static void outerLoop();

    /**
     * Controls rotation.
     */
    static void rotationLoop();

    /**
     * Calibrates the MPU6050.
     * Outputs values into accXoffset etc.
     */
    static void calibrate();

    // Class members
    static IWifi *wifi;
    static MPU6050 mpu;
    static ESP32Timer ITimer;
    static Step step1;
    static Step step2;
    static PidParams params;
    static PidDirection direction;
    static SemaphoreHandle_t paramsMutex;
    static SemaphoreHandle_t directionMutex;

    // Class constants
    static constexpr int STEPPER_INTERVAL_US = 10;
    static constexpr float MOTOR_ACCEL_RAD = 30.0;
    static constexpr double LOOP_INTERVAL = 10;
    static constexpr double LOOP2_INTERVAL = 200;

    // Class variables
    static double accXoffset;
    static double accYoffset;
    static double accZoffset;
    static double gyroXoffset;
    static double gyroYoffset;
    static double angle_setpoint; // inner loop setpoint
    static double filtered_value;
    static double rotation_correction;
};

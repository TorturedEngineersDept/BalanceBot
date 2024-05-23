#include "PidController.h"

// Initialise static class variables
MPU6050 PidController::mpu;
ESP32Timer PidController::ITimer(PID_TIMER_NO);
Step PidController::step1(STEPPER_INTERVAL_US, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
Step PidController::step2(STEPPER_INTERVAL_US, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);

void PidController::setup()
{
    mpu.begin();
    pinMode(TOGGLE_PIN, OUTPUT);

    // Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
    if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, timerHandler))
    {
        Serial.println("Failed to start stepper interrupt");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("Initialised Interrupt for Stepper");

    // Set motor acceleration values
    step1.setAccelerationRad(MOTOR_ACCEL_RAD);
    step2.setAccelerationRad(MOTOR_ACCEL_RAD);

    // Enable the stepper motor drivers
    pinMode(STEPPER_EN, OUTPUT);
    digitalWrite(STEPPER_EN, false);
}

void PidController::loop()
{
    // Static variables are initialised once and then the value is remembered
    // betweeen subsequent calls to this function
    static unsigned long printTimer = 0; // time of the next print
    static unsigned long loopTimer = 0;  // time of the next control update
    static double tiltx = 0.0;
    static double gyrox = 0.0; // current tilt angle
    static double gyroangle = 0.0;
    static double theta_n = 0.0;

    const float kp = 2;
    // potentially don't need if we get the setpoint right
    // (correct offset calibration)
    const float ki = 0.002;
    const float kd = 1;
    const float setpoint = -0.51;
    double error;
    double previous_error = 0;
    double integral = 0;
    double derivative = 0;
    double previous_derivative = 0;
    double Pout, Iout, Dout, motor_out;

    // Run the control loop every LOOP_INTERVAL ms
    if (millis() > loopTimer)
    {
        loopTimer += LOOP_INTERVAL;

        // Fetch data from MPU6050
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        // Calculate Tilt using accelerometer and sin x = x approximation for
        // a small tilt angle
        tiltx = asin(a.acceleration.z / 9.81) - 0.05;

        gyrox = g.gyro.y - 0.04;
        gyroangle = gyroangle + (gyrox * (LOOP_INTERVAL));

        theta_n = 0.04 * (tiltx * 100) + 0.96 * ((gyrox * LOOP_INTERVAL) / 10 + theta_n);

        // PIDeez Nuts
        error = setpoint - theta_n;
        Pout = kp * error;

        integral = integral + error * (LOOP_INTERVAL / 1000);
        Iout = ki * integral;

        derivative = ((error - previous_error) / LOOP_INTERVAL) * 1000;
        derivative = previous_derivative + 0.2 * (derivative - previous_derivative);
        previous_derivative = derivative;

        Dout = kd * derivative;
        motor_out = Pout + Iout + Dout;
        previous_error = error;

        // Set target motor speed
        step1.setTargetSpeedRad(motor_out);
        step2.setTargetSpeedRad(-motor_out);
    }
}

bool PidController::timerHandler(void *args)
{
    static bool toggle = false;

    // Update the stepper motors
    step1.runStepper();
    step2.runStepper();

    // Indicate that the ISR is running
    digitalWrite(TOGGLE_PIN, toggle);
    toggle = !toggle;
    return true;
}

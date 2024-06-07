#include "PidController.h"

// Initialise static class variables
IWifi *PidController::wifi = nullptr;
MPU6050 PidController::mpu;
ESP32Timer PidController::ITimer(PID_TIMER_NO);
Step PidController::step1(STEPPER_INTERVAL_US, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
Step PidController::step2(STEPPER_INTERVAL_US, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);
SemaphoreHandle_t PidController::paramsMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t PidController::directionMutex = xSemaphoreCreateMutex();

// Initialise PID parameters using known values
PidParams PidController::params(1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00);
PidDirection PidController::direction(0, KeyDirection::STOP);

void PidController::setup(IWifi &wifi)
{
    // Set the wifi interface
    PidController::wifi = &wifi;

    mpu.begin(ULONG_MAX, wifi);
    pinMode(TOGGLE_PIN, OUTPUT);

    // Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
    if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, timerHandler))
    {
        wifi.println("Failed to start stepper interrupt");
        while (1)
        {
            delay(10);
        }
    }
    wifi.println("Initialised Interrupt for Stepper");

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
    static double theta_n = 0.0;

    // Fetch PID parameters - take mutex and wait forever until it is available
    xSemaphoreTake(paramsMutex, portMAX_DELAY);
    float kp_i = params.kp_i;
    float ki_i = params.ki_i;
    float kd_i = params.kd_i;
    float tilt_setpoint = params.tilt_setpoint;
    xSemaphoreGive(paramsMutex);

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

        const float ACCELOREMETER_OFFSET = 0.03;
        const float GYRO_OFFSET = -0.05;
        const float COMP_FILTER_COEFF = 0.98;

        // Calculate Tilt using accelerometer and sin x = x approximation for
        // a small tilt angle
        tiltx = a.acceleration.z / 9.81 - ACCELOREMETER_OFFSET;

        gyrox = g.gyro.y - GYRO_OFFSET;

        theta_n = (1 - COMP_FILTER_COEFF) * (tiltx * 100) + COMP_FILTER_COEFF * ((gyrox * LOOP_INTERVAL) / 10 + theta_n);

        // PIDeez Nuts
        error = tilt_setpoint - theta_n;
        Pout = kp_i * error;

        integral = integral + error * (LOOP_INTERVAL / 1000);
        Iout = ki_i * integral;

        derivative = ((error - previous_error) / LOOP_INTERVAL) * 1000;
        derivative = previous_derivative * (derivative - previous_derivative);
        previous_derivative = derivative;

        Dout = kd_i * derivative;
        motor_out = Pout + Iout + Dout;
        previous_error = error;

        // Set target motor speed
        step1.setTargetSpeedRad(motor_out);
        step2.setTargetSpeedRad(-motor_out);
    }
}

void PidController::stabilizedLoop()
{
    // We don't care about PID stability here.
    // Just get the robot moving
    // Maybe someone can be bothered to research the maths here

    // Static variable to store the last direction
    static PidDirection lastDirection = PidController::getDirection();

    // Get the current direction
    PidDirection currentDirection = PidController::getDirection();

    // Update last direction only if current direction is different
    if (currentDirection.key_dir != lastDirection.key_dir)
    {
        lastDirection = currentDirection;
    }

    float speed = lastDirection.speed;
    KeyDirection key_dir = lastDirection.key_dir;

    const float SPEED = 10;

    switch (key_dir)
    {
    case KeyDirection::RIGHT:
        step1.setTargetSpeedRad(SPEED);
        step2.setTargetSpeedRad(SPEED);
        Serial.println("RIGHT");
        break;
    case KeyDirection::FORWARD:
        step1.setTargetSpeedRad(-SPEED);
        step2.setTargetSpeedRad(SPEED);
        Serial.println("FORWARD");
        break;
    case KeyDirection::LEFT:
        step1.setTargetSpeedRad(-SPEED);
        step2.setTargetSpeedRad(-SPEED);
        Serial.println("LEFT");
        break;
    case KeyDirection::BACKWARD:
        step1.setTargetSpeedRad(SPEED);
        step2.setTargetSpeedRad(-SPEED);
        Serial.println("BACKWARDS");
        break;
    case KeyDirection::STOP:
    default:
        step1.setTargetSpeedRad(0);
        step2.setTargetSpeedRad(0);
        Serial.println("STOP");
    }
}

void PidController::setParams(PidParams params)
{
    // Take mutex and wait forever until it is available
    xSemaphoreTake(paramsMutex, portMAX_DELAY);
    PidController::params = params;
    xSemaphoreGive(paramsMutex);
}

PidParams PidController::getParams()
{
    xSemaphoreTake(paramsMutex, portMAX_DELAY);
    PidParams p = params;
    xSemaphoreGive(paramsMutex);
    return p;
}

void PidController::setDirection(PidDirection direction)
{
    // Take mutex and wait forever until it is available
    xSemaphoreTake(directionMutex, portMAX_DELAY);
    PidController::direction = direction;
    xSemaphoreGive(directionMutex);
}

PidDirection PidController::getDirection()
{
    xSemaphoreTake(directionMutex, portMAX_DELAY);
    PidDirection d = direction;
    xSemaphoreGive(directionMutex);
    return d;
}

// Interrupt Service Routine for motor update
// Note: ESP32 doesn't support floating point calculations in an ISR
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

#include "PidController.h"

// Initialise static class variables
IWifi *PidController::wifi = nullptr;
MPU6050 PidController::mpu;
ESP32Timer PidController::ITimer(PID_TIMER_NO);
Step PidController::step1(STEPPER_INTERVAL_US, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
Step PidController::step2(STEPPER_INTERVAL_US, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);
SemaphoreHandle_t PidController::paramsMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t PidController::directionMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t PidController::controlMutex = xSemaphoreCreateMutex();

double PidController::accXoffset = 0;
double PidController::accYoffset = 0;
double PidController::accZoffset = 0;
double PidController::gyroXoffset = 0;
double PidController::gyroYoffset = 0;
double PidController::angle_setpoint = 0;
double PidController::filtered_value = 0;
double PidController::rotation_correction = 0;

// Initialise PID parameters using known values
PidParams PidController::params(1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00);
PidDirection PidController::direction(0, KeyDirection::STOP);

bool PidController::setup(IWifi &wifi, unsigned long timeout)
{
    // Set the wifi interface
    PidController::wifi = &wifi;
    unsigned long start = 0;

    // Timeout not entirely accurate, but good enough
    if (!mpu.begin(timeout, wifi))
    {
        return false;
    }

    pinMode(TOGGLE_PIN, OUTPUT);

    // Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
    if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, timerHandler))
    {
        wifi.println("Failed to start stepper interrupt");
        while (start < timeout)
        {
            delay(10);
            start += 10;
        }
    }

    if (start >= timeout)
    {
        return false;
    }

    calibrate();

    wifi.println("Initialised Interrupt for Stepper");

    // Set motor acceleration values
    step1.setAccelerationRad(MOTOR_ACCEL_RAD);
    step2.setAccelerationRad(MOTOR_ACCEL_RAD);

    // Enable the stepper motor drivers
    pinMode(STEPPER_EN, OUTPUT);
    digitalWrite(STEPPER_EN, false);

    return true;
}

void PidController::loop()
{
    // Time of the next control update
    static unsigned long loop1Timer = 0; // inner
    static unsigned long loop2Timer = 0; // outer
    static unsigned long loop3Timer = 0; // rotation loop

    // Run the control loop every LOOP_INTERVAL ms
    if (millis() > loop1Timer)
    {
        innerLoop();
        loop1Timer += LOOP_INTERVAL;
    }

    if (millis() > loop2Timer)
    {
        outerLoop();
        loop2Timer += LOOP2_INTERVAL;
    }
}

void PidController::stabilizedLoop()
{
    // We don't care about PID stability here.
    // Just get the robot moving
    // Maybe someone can be bothered to research the maths here

    PidDirection currentDirection = PidController::getDirection();
    KeyDirection key_dir = currentDirection.key_dir;

    const float SPEED = 10;

    switch (key_dir)
    {
    case KeyDirection::RIGHT:
        step1.setTargetSpeedRad(SPEED);
        step2.setTargetSpeedRad(SPEED);
        // Serial.println("RIGHT");
        break;
    case KeyDirection::FORWARD:
        step1.setTargetSpeedRad(-SPEED);
        step2.setTargetSpeedRad(SPEED);
        // Serial.println("FORWARD");
        break;
    case KeyDirection::LEFT:
        step1.setTargetSpeedRad(-SPEED);
        step2.setTargetSpeedRad(-SPEED);
        // Serial.println("LEFT");
        break;
    case KeyDirection::BACKWARD:
        step1.setTargetSpeedRad(SPEED);
        step2.setTargetSpeedRad(-SPEED);
        // Serial.println("BACKWARDS");
        break;
    case KeyDirection::STOP:
    default:
        step1.setTargetSpeedRad(0);
        step2.setTargetSpeedRad(0);
        // Serial.println("STOP");
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

void PidController::innerLoop()
{
    // Static variables are initialised once and then the value is remembered
    // betweeen subsequent calls to this function
    static double previous_angle_error = 0;
    static double robot_angle = 0.0;

    const double COMP_FILTER = 0.98;  // complementary filter coefficient
    const double LOOP1_INTERVAL = 10; // inner loop
    const double ALPHA = 0.05;        // filter coefficient
    const double MOTOR_SPEED = 17;

    // Fetch PID parameters - take mutex and wait forever until it is available
    xSemaphoreTake(paramsMutex, portMAX_DELAY);
    float kp_i = params.kp_i;
    float ki_i = params.ki_i;
    float kd_i = params.kd_i;
    float tilt_setpoint = params.tilt_setpoint;
    xSemaphoreGive(paramsMutex);

    // Fetch data from MPU6050
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    double accX = a.acceleration.x - accXoffset;
    double accY = a.acceleration.y - accYoffset;
    double accZ = a.acceleration.z - accZoffset;

    // Calculate Tilt using accelerometer and sin x = x approximation for a
    // small tilt angle - tiltx = (accZ) / (9.67);
    double tiltx = (accZ) / sqrt(pow(accY, 2) + pow(accX, 2));

    double gyro_angle = g.gyro.y - gyroYoffset;

    robot_angle = (1 - COMP_FILTER) * (tiltx) + COMP_FILTER;
    robot_angle *= ((gyro_angle * (LOOP1_INTERVAL / 1000)) + robot_angle);

    // PIDeez Nuts
    double angle_error = angle_setpoint - robot_angle * 57.32;
    double Pout_a = kp_i * angle_error;
    double integral_angle = integral_angle + ((angle_error * LOOP1_INTERVAL) / 1000);
    double Iout_a = ki_i * integral_angle;
    double Dout_a = kd_i * ((angle_error - previous_angle_error) / LOOP1_INTERVAL) * 1000;
    double motor_out = Pout_a + Iout_a + Dout_a;

    previous_angle_error = angle_error;

    if (motor_out >= 0)
    {
        step1.setTargetSpeedRad(MOTOR_SPEED);
        step2.setTargetSpeedRad(-MOTOR_SPEED);
    }
    else
    {
        step1.setTargetSpeedRad(-MOTOR_SPEED);
        step2.setTargetSpeedRad(MOTOR_SPEED);
    }

    // uses absolute values
    step1.setAccelerationRad(motor_out + rotation_correction);
    step2.setAccelerationRad(motor_out - rotation_correction);

    double avgSpeed = (step1.getSpeedRad() - step2.getSpeedRad()) / 2;
    filtered_value = ALPHA * avgSpeed + (1 - ALPHA) * filtered_value;
}

void PidController::outerLoop()
{
    static double avgspeed = 0, avg_temp = 0, speed_error = 0, previous_speed_error = 0, integral_speed = 0, derivative_speed = 0, Pout_s = 0, Iout_s = 0, Dout_s = 0, ramp_rate = 0;
    static double current_speed_setpoint = 0;
    static double speed_setpoint = 0; // outer loop setpoint
    static int speed_size = 0;

    double max_ramp_rate = 0.5;
    double time_constant = 2000;
    double beta = 1 - exp(-LOOP2_INTERVAL / time_constant);

    // Fetch PID parameters - take mutex and wait forever until it is available
    xSemaphoreTake(paramsMutex, portMAX_DELAY);
    float kp_o = params.kp_o;
    float ki_o = params.ki_o;
    float kd_o = params.kd_o;
    float balancing_setpoint = params.velocity_setpoint;

    current_speed_setpoint += beta * (speed_setpoint - current_speed_setpoint);

    // filtered_value = (step1.getSpeedRad()-step2.getSpeedRad())/2;
    speed_error = -(current_speed_setpoint - filtered_value);

    Pout_s = kp_o * speed_error;

    integral_speed = integral_speed + speed_error * (LOOP2_INTERVAL / 1000);

    if (ki_o == 0)
    {
        integral_speed = 0;
    }

    Iout_s = ki_o * integral_speed;

    derivative_speed = ((speed_error - previous_speed_error) / LOOP2_INTERVAL) * 1000;

    Dout_s = kd_o * derivative_speed;

    angle_setpoint = Pout_s + Iout_s + Dout_s + balancing_setpoint;

    if (speed_setpoint > 0)
    {
        if (angle_setpoint > balancing_setpoint + 1)
        {
            angle_setpoint = balancing_setpoint + 1;
        }
        else if (angle_setpoint < balancing_setpoint - 5)
        {
            angle_setpoint = balancing_setpoint - 5;
        }
    }

    else if (speed_setpoint < 0)
    {
        if (angle_setpoint < balancing_setpoint - 1)
        {
            angle_setpoint = balancing_setpoint - 1;
        }
        else if (angle_setpoint > balancing_setpoint + 5)
        {
            angle_setpoint = balancing_setpoint + 5;
        }
    }

    previous_speed_error = speed_error;
}

void PidController::rotationLoop()
{
}

void PidController::calibrate()
{
    sensors_event_t a, g, temp;
    double accXoffset_sum = 0;
    double accYoffset_sum = 0;
    double accZoffset_sum = 0;
    double gyroXoffset_sum = 0;
    double gyroYoffset_sum = 0;

    for (int i = 0; i < 200; i++)
    {
        mpu.getEvent(&a, &g, &temp);
        accXoffset_sum += a.acceleration.x;
        accYoffset_sum += a.acceleration.y;
        accZoffset_sum += a.acceleration.z;
        gyroXoffset_sum += g.gyro.x;
        gyroYoffset_sum += g.gyro.y;
    }

    accXoffset = accXoffset_sum / 200 - 9.81;
    accYoffset = accYoffset_sum / 200;
    accZoffset = accZoffset_sum / 200;
    gyroXoffset = gyroXoffset_sum / 200;
    gyroYoffset = gyroYoffset_sum / 200;
}

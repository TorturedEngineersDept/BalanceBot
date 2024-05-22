#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

Adafruit_MPU6050 mpu;

void setUp(void)
{
    // This will be run before each test
    Wire.begin();
    TEST_ASSERT_TRUE_MESSAGE(mpu.begin(), "Failed to initialize MPU6050");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

void tearDown(void)
{
    // This will be run after each test
    // Any cleanup code can be put here
}

void test_accelerometer_values(void)
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0.0, a.acceleration.x,
                                  "Acceleration X is zero");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0.0, a.acceleration.y,
                                  "Acceleration Y is zero");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0.0, a.acceleration.z,
                                  "Acceleration Z is zero");
}

void process()
{
    UNITY_BEGIN();
    RUN_TEST(test_accelerometer_values);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup()
{
    delay(2000);  // Allow some time for the serial port to initialize
    process();
}

void loop()
{
    // Not used in this test
}

#else

int main(int argc, char **argv)
{
    process();
    return 0;
}

#endif
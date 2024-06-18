#include "MPU6050.h"
#include "WifiSetup.h"
#include <Arduino.h>
#include <Wire.h>
#include <unity.h>

MPU6050 mpu;
// Declare variables for RunID and BotID
int BotID, RunID;

void setUp(void)
{
    // This will be run before each test
    Wire.begin();
    WifiSetup wifi("ssid", "password", "mqtt_server", 1883);
    TEST_ASSERT_TRUE_MESSAGE(mpu.begin(100, wifi), "Failed to initialize MPU6050");
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
    TEST_ASSERT_TRUE_MESSAGE(a.acceleration.x != 0.0 ||
                                 a.acceleration.y != 0.0 ||
                                 a.acceleration.z != 0.0,
                             "Accelerometer output is 0,0,0");
}

void setup()
{
    delay(2000); // Allow some time for the serial port to initialize
    UNITY_BEGIN();
    RUN_TEST(test_accelerometer_values);
}

void loop()
{
    UNITY_END();
}

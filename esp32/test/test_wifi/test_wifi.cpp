/**
 * Important note:
 *
 * Use `g_ssid` and `g_password` to connect to the WiFi network, because the CI
 * test environment will be at a different location than your local machine.
 */

#include <Arduino.h>
#include "WifiSetup.h"
#include <unity.h>

// Declare variables for RunID and BotID
int BotID, RunID;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_wifi_connect()
{
    WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(WL_CONNECTED, WiFi.status());
}

void test_mqtt_available()
{
    WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(0, wifi.pingServer(MQTT_SERVER));
}

void test_mqtt_connect()
{
    WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(true, wifi.mqttConnected());
}

void test_wifi_disconnect()
{
    const char *wrong_password = "wrong_password";
    WifiSetup wifi(g_ssid, wrong_password, MQTT_SERVER, MQTT_PORT);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(WL_DISCONNECTED, WiFi.status());
}

void test_remote_joystick()
{
    WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);

    // No need to connect to WiFi here but make RunID available
    RunID = 121;

    // Use the callback
    char *topic = const_cast<char *>("user/joystick");
    char *payload = const_cast<char *>(R"({"run_id": 121, "direction": 'A'})");
    wifi.callback(topic, reinterpret_cast<byte *>(payload), strlen(payload));

    // Default speed = 100
    PidDirection expectedDirection = PidDirection(100, KeyDirection::LEFT);
    PidDirection actualDirection = PidController::getDirection();

    TEST_ASSERT_EQUAL(expectedDirection.speed, actualDirection.speed);
    TEST_ASSERT_EQUAL(expectedDirection.key_dir, actualDirection.key_dir);
}

void test_remote_pid()
{
    WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);

    // No need to connect to WiFi here but make RunID available
    RunID = 121;

    // Use the callback
    char *topic = const_cast<char *>("user/pid");
    char *payload = const_cast<char *>(
        R"({
            "run_id": 121, 
            "kp_i": 0.5, 
            "ki_i": 0.1, 
            "kd_i": 0.2, 
            "tilt_setpoint": 30.0, 
            "kp_o": 0.5, 
            "ki_o": 0.1, 
            "kd_o": 0.2, 
            "velocity_setpoint": 30, 
            "rotation_setpoint": 0.4
        })");
    wifi.callback(topic, reinterpret_cast<byte *>(payload), strlen(payload));

    PidParams actualParams = PidController::getParams();

    TEST_ASSERT_EQUAL(0.5, actualParams.kp_i);
    TEST_ASSERT_EQUAL(0.1, actualParams.ki_i);
    TEST_ASSERT_EQUAL(0.2, actualParams.kd_i);
    TEST_ASSERT_EQUAL(30, actualParams.tilt_setpoint);
    TEST_ASSERT_EQUAL(0.5, actualParams.kp_o);
    TEST_ASSERT_EQUAL(0.1, actualParams.ki_o);
    TEST_ASSERT_EQUAL(0.2, actualParams.kd_o);
    TEST_ASSERT_EQUAL(30, actualParams.velocity_setpoint);
    TEST_ASSERT_EQUAL(0.4, actualParams.rotation_setpoint);
}

void setup()
{
    delay(2000);   // delay to allow opening of Serial Monitor
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_wifi_connect);
    RUN_TEST(test_mqtt_connect);
    RUN_TEST(test_wifi_disconnect);
    RUN_TEST(test_remote_joystick);
    RUN_TEST(test_remote_pid);
}

void loop()
{
    UNITY_END(); // stop unit testing
}

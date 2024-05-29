/**
 * Important note:
 *
 * Use `g_ssid` and `g_password` to connect to the WiFi network, because the CI
 * test environment will be at a different location than your local machine.
 */

#include <Arduino.h>
#include "WifiSetup.h"
#include <unity.h>

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

    // No need to connect to WiFi here

    // Use the callback
    char *topic = const_cast<char *>("user/joystick");
    char *payload = const_cast<char *>(R"({"speed": 0.5, "angle": 30})");
    wifi.callback(topic, reinterpret_cast<byte *>(payload), strlen(payload));

    PidDirection expectedDirection = PidDirection(0.5, 30);
    PidDirection actualDirection = PidController::getDirection();

    TEST_ASSERT_EQUAL(expectedDirection.speed, actualDirection.speed);
    TEST_ASSERT_EQUAL(expectedDirection.angle, actualDirection.angle);
}

void test_remote_pid()
{
    WifiSetup wifi(g_ssid, g_password, MQTT_SERVER, MQTT_PORT);

    // No need to connect to WiFi here

    // Use the callback
    char *topic = const_cast<char *>("user/pid");
    char *payload = const_cast<char *>(
        R"({"Kp": 0.5, "Ki": 0.1, "Kd": 0.2, "setpoint": 30})");
    wifi.callback(topic, reinterpret_cast<byte *>(payload), strlen(payload));

    PidParams expectedParams = PidParams(0.5, 0.1, 0.2, 30);
    PidParams actualParams = PidController::getParams();

    TEST_ASSERT_EQUAL(expectedParams.Kp, actualParams.Kp);
    TEST_ASSERT_EQUAL(expectedParams.Ki, actualParams.Ki);
    TEST_ASSERT_EQUAL(expectedParams.Kd, actualParams.Kd);
    TEST_ASSERT_EQUAL(expectedParams.setpoint, actualParams.setpoint);
}

void setup()
{
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

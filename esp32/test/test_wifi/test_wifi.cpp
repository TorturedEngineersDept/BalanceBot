#include <Arduino.h>
#include "WifiSetup.h"
#include <unity.h>
#include <future>
#include <chrono>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_wifi_connect()
{
    WifiSetup wifi(g_ssid, g_password);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(WL_CONNECTED, WiFi.status());
}

void test_mqtt_available()
{
    WifiSetup wifi(g_ssid, g_password);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(0, wifi.pingServer());
}

void test_mqtt_connect()
{
    WifiSetup wifi(g_ssid, g_password);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(true, wifi.mqttConnected());
}

void test_wifi_disconnect()
{
    const char *wrong_password = "wrong_password";
    WifiSetup wifi(g_ssid, wrong_password);

    // Setup WiFi
    wifi.connect(3000);

    TEST_ASSERT_EQUAL(WL_DISCONNECTED, WiFi.status());
}

void setup()
{
    UNITY_BEGIN(); // IMPORTANT LINE!
    RUN_TEST(test_wifi_connect);
    RUN_TEST(test_mqtt_connect);
    RUN_TEST(test_wifi_disconnect);
}

void loop()
{
    UNITY_END(); // stop unit testing
}
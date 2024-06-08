#pragma once

#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "MqttSetup.h"
#include "PidController.h"
#include "Passwords.h"
#include "IWifi.h"

#define MQTT_SERVER "18.130.87.186"
#define MQTT_PORT 1883

extern int RunID;
extern int BotID;
class WifiSetup : public IWifi
{
public:
    WifiSetup(
        const char *ssid,
        const char *password,
        const char *mqtt_server = MQTT_SERVER,
        int mqtt_port = MQTT_PORT);

    WifiSetup(
        const char *ssid,
        const char *username,
        const char *password,
        const char *mqtt_server = MQTT_SERVER,
        int mqtt_port = MQTT_PORT);

    /**
     * Connect to the WiFi network
     * @param timeout The maximum time to wait for the connection (ms)
     */
    void connect(unsigned long timeout = ULONG_MAX);

    /**
     * Ping the server to ensure it's reachable. If the server is unreachable,
     * the function will attempt to ping Google (8.8.8.8).
     * @param server The server to ping
     * @return The ping status (0 = success, 1 = fail, 2 = double fail)
     */
    int pingServer(const char *server = MQTT_SERVER) const;

    /**
     * Check if the MQTT client is connected.
     * @return True if the client is connected, false otherwise
     */
    bool mqttConnected();

    /**
     * Loop function to be called in the main loop.
     */
    void loop();

    /**
     * Sends a message to the MQTT server, to print debug information.
     * Also prints the message to the serial monitor.
     */
    void print(const char *message) override;

    /**
     * Sends a message to the MQTT server, to print debug information.
     * Also prints the message to the serial monitor.
     */
    void println(const char *message) override;

    /**
     * Callback function for MQTT messages.
     * Public to be testable.
     */
    static void callback(char *topic, byte *payload, unsigned int length);

    /**
     * Get unique BotId and RunId
     */
    void resolveId();

private:
    void getStrength() const;

    const char *ssid;
    const char *username;
    const char *password;
    MqttSetup mqtt;
    bool runIdResolved = false;
};

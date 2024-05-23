#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>
#include <MqttSetup.h>
#include <Passwords.h>

#define mqtt_server "18.132.10.124"
#define mqtt_port 1883

class WifiSetup
{
public:
    WifiSetup(const char *ssid, const char *password);

    /**
     * Connect to the WiFi network
     * @param timeout The maximum time to wait for the connection (ms)
     */
    void connect(unsigned long timeout = ULONG_MAX);

    void getStrength() const;

    /**
     * Ping the server to ensure it's reachable. If the server is unreachable,
     * the function will attempt to ping Google (8.8.8.8).
     * @param server The server to ping
     * @return The ping status (0 = success, 1 = fail, 2 = double fail)
     */
    int pingServer(const char *server = mqtt_server) const;

    bool mqttConnected();

    void loop();

private:
    static void callback(char *topic, byte *payload, unsigned int length);

    const char *ssid;
    const char *password;
    MqttSetup mqtt;
};

#endif // WIFI_SETUP_H

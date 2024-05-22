#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>
#include <Passwords.h>

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
    void pingServer(const char *server) const;

private:
    const char *ssid;
    const char *password;
};

#endif // WIFI_SETUP_H

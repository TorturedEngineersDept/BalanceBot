#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>

class WifiSetup
{
public:
    WifiSetup(const char *ssid, const char *password);
    void connect();
    void getStrength() const;
    void pingServer(const char *server) const;

private: 
    const char *ssid;
    const char *password;
};

#endif // WIFI_SETUP_H

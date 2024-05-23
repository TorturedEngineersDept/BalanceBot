#include "WifiSetup.h"
#include "ESP32Ping.h"

WifiSetup::WifiSetup(const char *ssid, const char *password)
    : ssid(ssid), password(password) {}

void WifiSetup::connect(unsigned long timeout)
{
    delay(10);
    // Start connecting to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    unsigned long start = 0;
    uint32_t d = 500;

    while (WiFi.status() != WL_CONNECTED && start < timeout)
    {
        delay(d);
        Serial.print(".");
        start += d;
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void WifiSetup::getStrength() const
{
    Serial.print("Signal strength (RSSI): ");
    Serial.println(WiFi.RSSI());
}

void WifiSetup::pingServer(const char *server) const
{
    IPAddress ip;
    if (!WiFi.hostByName(server, ip))
    {
        Serial.println("DNS lookup failed. Unable to resolve the MQTT broker.");
        return;
    }

    Serial.print("Pinging MQTT broker at ");
    Serial.println(ip);

    if (Ping.ping(ip))
    {
        Serial.println("Ping successful.");
    }
    else
    {
        Serial.println("Ping failed.");
    }

    // Test ping to Google DNS
    Serial.println("Trying ping to Google server");
    if (Ping.ping(IPAddress(8, 8, 8, 8)))
    {
        Serial.println("Ping to 8.8.8.8 successful.");
    }
    else
    {
        Serial.println("Ping to 8.8.8.8 failed.");
    }
}
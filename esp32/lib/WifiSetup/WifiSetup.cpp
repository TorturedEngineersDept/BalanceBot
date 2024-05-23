#include "WifiSetup.h"
#include "ESP32Ping.h"

WifiSetup::WifiSetup(const char *ssid, const char *password)
    : ssid(ssid), password(password), mqtt(mqtt_server, mqtt_port)
{
}

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

    // Get the WiFi signal strength
    getStrength();

    // Ping the MQTT server to ensure it's reachable
    pingServer(mqtt_server);

    // Setup MQTT
    mqtt.setCallback(callback);
    mqtt.connect(timeout - start);
}

void WifiSetup::getStrength() const
{
    Serial.print("Signal strength (RSSI): ");
    Serial.println(WiFi.RSSI());
}

int WifiSetup::pingServer(const char *server) const
{
    IPAddress ip;
    if (!WiFi.hostByName(server, ip))
    {
        Serial.println("DNS lookup failed. Unable to resolve the MQTT broker.");
        return 1;
    }

    Serial.print("Pinging MQTT broker at ");
    Serial.println(ip);

    if (Ping.ping(ip))
    {
        Serial.println("Ping successful.");
        return 0;
    }

    Serial.println("Ping failed.");

    // Test ping to Google DNS
    Serial.println("Trying ping to Google server");
    if (Ping.ping(IPAddress(8, 8, 8, 8)))
    {
        Serial.println("Ping to 8.8.8.8 successful.");
        return 1;
    }

    Serial.println("Ping to 8.8.8.8 failed.");
    return 2;
}

bool WifiSetup::mqttConnected()
{
    return mqtt.isConnected();
}

void WifiSetup::loop()
{
    mqtt.loop();
}

void WifiSetup::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Create a buffer to store the payload
    char payloadStr[length + 1];
    memcpy(payloadStr, payload, length);
    payloadStr[length] = '\0'; // Null-terminate the string

    Serial.println(payloadStr);

    // Parse the JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payloadStr);

    if (error)
    {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
    }

    // Check the topic and process accordingly
    if (strcmp(topic, "user/joystick") == 0)
    {
        // Extract values from the JSON document
        float speed = doc["speed"];
        float angle = doc["angle"];
        Serial.println("Speed: " + String(speed) + ", Angle: " + String(angle));

        // TODO: Processing logic here
    }
    else
    {
        Serial.println("Unsupported topic");
    }
}
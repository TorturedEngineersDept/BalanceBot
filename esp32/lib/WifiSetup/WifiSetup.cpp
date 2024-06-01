#include "WifiSetup.h"
#include "ESP32Ping.h"

WifiSetup::WifiSetup(
    const char *ssid,
    const char *password,
    const char *mqtt_server,
    int mqtt_port)
    : ssid(ssid),
      username(nullptr),
      password(password),
      mqtt(mqtt_server, mqtt_port)
{
}

WifiSetup::WifiSetup(
    const char *ssid,
    const char *username,
    const char *password,
    const char *mqtt_server,
    int mqtt_port)
    : ssid(ssid),
      username(username),
      password(password),
      mqtt(mqtt_server, mqtt_port)
{
}

void WifiSetup::connect(unsigned long timeout)
{
    delay(10);
    // Start connecting to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    if (username != nullptr)
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, WPA2_AUTH_PEAP, username, username, password);
    }
    else
    {
        WiFi.begin(ssid, password);
    }

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
    pingServer(mqtt.getServer());

    // Setup MQTT
    mqtt.setCallback(callback);
    mqtt.connect(timeout - start);

    // Done
    connected = true;
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

void WifiSetup::print(const char *message)
{
    if (connected)
    {
        DebugMessage debugMessage(message);
        mqtt.publishMessage(debugMessage);
    }
    Serial.print(message);
}

void WifiSetup::println(const char *message)
{
    if (connected)
    {
        std::string msg = std::string(message) + "\n";
        DebugMessage debugMessage(msg.c_str());
        mqtt.publishMessage(debugMessage);
    }
    Serial.println(message);
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

        PidController::setDirection(PidDirection(speed, angle));
    }
    else if (strcmp(topic, "user/pid") == 0)
    {
        // Extract values from the JSON document
        float kp_i = doc["kp_i"];
        float ki_i = doc["ki_i"];
        float kd_i = doc["kd_i"];
        float setpoint_i = doc["setpoint_i"];
        float kp_o = doc["kp_o"];
        float ki_o = doc["ki_o"];
        float kd_o = doc["kd_o"];
        float setpoint_o = doc["setpoint_o"];
        Serial.println("kp_i: " + String(kp_i) +
                       ", ki_i: " + String(ki_i) +
                       ", kd_i: " + String(kd_i) +
                       ", Setpoint_i: " + String(setpoint_i) +
                       ", kp_o: " + String(kp_o) +
                       ", ki_o: " + String(ki_o) +
                       ", kd_o: " + String(kd_o) +
                       ", Setpoint_o: " + String(setpoint_o));

        // Use the callback given in the static class
        PidController::setParams(PidParams(kp_i, ki_i, kd_i, setpoint_i, kp_o, ki_o, kd_o, setpoint_o));
    }
    else
    {
        Serial.println("Unsupported topic");
    }
}

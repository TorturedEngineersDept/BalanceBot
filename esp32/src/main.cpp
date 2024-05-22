#include <Arduino.h>
#include "WifiSetup.h"
#include "MqttSetup.h"
#include "BatteryMessage.h"
#include "ESP32Ping.h" // Include the ESP32Ping library

const char *ssid = "Sara";
const char *password = "hello231";
const char *mqtt_server = "18.132.10.124";
int mqtt_port = 1883;

int batteryLevel = 100;
float speed;
float angle;

WifiSetup wifi(ssid, password);
MqttSetup mqtt(mqtt_server, mqtt_port);

int lastMsgSent = 0;
int delayMsgSent = 2000;

void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
    Serial.begin(115200);

    // Setup WiFi
    Serial.println("Connecting to WiFi...");
    wifi.connect();
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed to connect to WiFi.");
        return;
    }
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.localIP());

    // Get the WiFi signal strength
    wifi.getStrength();

    // Ping the MQTT server to ensure it's reachable
    // wifi.pingServer(mqtt_server);

    // Setup MQTT
    mqtt.setCallback(callback);
    mqtt.connect();
}

void loop()
{
    // Ensure the client remains connected
    if (!mqtt.isConnected())
    {
        Serial.println("MQTT not connected, attempting to reconnect...");
        mqtt.connect();
    }

    // Send a new message every 2 seconds
    int current_time = millis();
    if (current_time - lastMsgSent > delayMsgSent)
    {
        lastMsgSent = millis();

        // Update battery status and publish message
        batteryLevel -= 7;
        if (batteryLevel < 0)
        {
            batteryLevel = 100;
        }
        BatteryMessage batteryMessage(batteryLevel);
        mqtt.publishMessage(batteryMessage);
    }

    mqtt.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
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

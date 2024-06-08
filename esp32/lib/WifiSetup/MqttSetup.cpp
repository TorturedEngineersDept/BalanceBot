#include "MqttSetup.h"

float x = 0;
float y = 0;

MqttSetup::MqttSetup(const char *server, int port)
    : server(server), port(port), client(espClient)
{
    client.setServer(server, port);
}

void MqttSetup::connect(unsigned long timeout)
{
    unsigned long start = 0;
    while (!client.connected() && start < timeout)
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client"))
        {
            client.setKeepAlive(60); // Set keep-alive to 60 seconds
            Serial.println("connected");
            client.subscribe("user/joystick");
            client.subscribe("user/pid");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 0.5 seconds");
            delay(500);
        }
    }

    // Send a status message to update run number on server
    StatusMessage statusMessage;
    publishMessage(statusMessage);
}

void MqttSetup::loop()
{
    // Ensure the client remains connected
    if (!isConnected())
    {
        Serial.println("MQTT not connected, attempting to reconnect...");
        connect();
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
        publishMessage(batteryMessage);

        // TODO: Update mapping status and publish message
        // MappingMessage mappingMessage(x, y, orientation * PI / 180);
        // publishMessage(mappingMessage);
    }

    client.loop();
}

void MqttSetup::setCallback(MQTT_CALLBACK_SIGNATURE)
{
    client.setCallback(callback);
}

void MqttSetup::publishMessage(MqttMessage &message)
{
    char buffer[128];
    message.toJson(buffer, sizeof(buffer));
    client.publish(message.getTopic(), buffer);
}

bool MqttSetup::isConnected()
{
    return client.connected();
}

const char *MqttSetup::getServer() const
{
    return server;
}

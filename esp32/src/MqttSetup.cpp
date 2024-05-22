#include "MqttSetup.h"

MqttSetup::MqttSetup(const char *server, int port)
    : server(server), port(port), client(espClient)
{
    client.setServer(server, port);
}

void MqttSetup::connect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client"))
        {
            Serial.println("connected");
            client.subscribe("user/joystick");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void MqttSetup::loop()
{
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
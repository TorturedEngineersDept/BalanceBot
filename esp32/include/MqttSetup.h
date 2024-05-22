#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "MqttMessage.h"

class MqttSetup
{
public:
    MqttSetup(const char *server, int port);
    void connect();
    void loop();
    void setCallback(MQTT_CALLBACK_SIGNATURE);
    void publishMessage(MqttMessage &message);
    bool isConnected();
    void pingServer() const;

private:
    const char *server;
    int port;
    WiFiClient espClient;
    PubSubClient client;
};

#endif // MQTT_SETUP_H

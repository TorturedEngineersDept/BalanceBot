#pragma once

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "MqttMessage.h"

extern int RunID;
extern int BotID;
class MqttSetup
{
public:
    MqttSetup(const char *server, int port);
    void connect(unsigned long timeout = ULONG_MAX);

    /**
     * Loop function that takes care of the sending of messages
     * over to the server
     */
    void loop();
    void setCallback(MQTT_CALLBACK_SIGNATURE);

    /**
     * Publish a message to the MQTT server.
     * MqttMessage must contain `toJson()`.
     */
    void publishMessage(MqttMessage &message);
    bool isConnected();
    void pingServer() const;
    const char *getServer() const;

private:
    const char *server;
    int port;
    WiFiClient espClient;
    PubSubClient client;

    int lastMsgSent = 0;
    int delayMsgSent = 2000;
    int batteryLevel = 100;
    float speed;
    float angle;
};

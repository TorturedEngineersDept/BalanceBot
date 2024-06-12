#pragma once

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include "MqttMessage.h"

extern int RunID;
extern int BotID;
class MqttSetup
{
public:
    MqttSetup(const char *server, int port, NTPClient &timeClient);
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

    unsigned long getEpochTime();

private:
    const char *server;
    int port;
    WiFiClient espClient;
    PubSubClient client;

    NTPClient &timeClient;

    int lastMsgSent = 0;
    int delayMsgSent = 2000;
    int batteryLevel = 100;

    char *client_id;
};

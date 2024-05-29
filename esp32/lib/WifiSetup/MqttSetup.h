#pragma once

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "MqttMessage.h"
#include "BatteryMessage.h"
#include "MappingMessage.h"
#include "StatusMessage.h"

class MqttSetup
{
public:
    MqttSetup(const char *server, int port);
    void connect(unsigned long timeout = ULONG_MAX);
    void loop();
    void setCallback(MQTT_CALLBACK_SIGNATURE);
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

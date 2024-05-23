#ifndef MQTT_SETUP_H
#define MQTT_SETUP_H

#include <PubSubClient.h>
#include <WiFiClient.h>
#include "MqttMessage.h"
#include "BatteryMessage.h"
#include "MappingMessage.h"

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

#endif // MQTT_SETUP_H

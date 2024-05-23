#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <ArduinoJson.h>

class MqttMessage
{
public:
    virtual void toJson(char *buffer, size_t bufferSize) = 0;
    virtual const char *getTopic() = 0;
};

#endif // MQTT_MESSAGE_H

#pragma once

#include <ArduinoJson.h>

class MqttMessage
{
public:
    virtual void toJson(char *buffer, size_t bufferSize) = 0;
    virtual const char *getTopic() = 0;
};

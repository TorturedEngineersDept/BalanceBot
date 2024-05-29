#pragma once

#include "MqttMessage.h"

class BatteryMessage : public MqttMessage
{
public:
    BatteryMessage(float batteryLevel);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    float batteryLevel;
    StaticJsonDocument<200> doc;
};

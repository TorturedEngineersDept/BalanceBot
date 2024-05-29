#pragma once

#include "MqttMessage.h"

class StatusMessage : public MqttMessage
{
public:
    StatusMessage();
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    StaticJsonDocument<200> doc;
};

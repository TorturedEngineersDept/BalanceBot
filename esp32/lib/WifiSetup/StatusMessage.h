#ifndef STATUS_MESSAGE_H
#define STATUS_MESSAGE_H

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

#endif // STATUS_MESSAGE_H

#ifndef MAPPING_MESSAGE_H
#define MAPPING_MESSAGE_H

#include "MqttMessage.h"

class MappingMessage : public MqttMessage
{
public:
    MappingMessage(float x, float y, float orientation);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    float x_coordinate;
    float y_coordinate;
    float orientation;
    StaticJsonDocument<200> doc;
};

#endif // MAPPING_MESSAGE_H
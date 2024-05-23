#include "MappingMessage.h"

MappingMessage::MappingMessage(float x, float y, float theta)
{
    x_coordinate = x;
    y_coordinate = y;
    orientation = theta;
}

void MappingMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["timestamp"] = millis();
    doc["x"] = x_coordinate;
    doc["y"] = y_coordinate;
    doc["oritentation"] = orientation;
    serializeJson(doc, buffer, bufferSize);
}

const char *MappingMessage::getTopic()
{
    return "esp32/mapping";
}
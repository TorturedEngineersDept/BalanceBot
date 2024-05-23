#include "StatusMessage.h"

StatusMessage::StatusMessage() {}

void StatusMessage::toJson(char *buffer, size_t bufferSize)
{
    doc["connection"] = 400;
    serializeJson(doc, buffer, bufferSize);
}

const char *StatusMessage::getTopic()
{
    return "esp32/status";
}

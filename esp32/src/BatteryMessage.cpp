#include "BatteryMessage.h"

BatteryMessage::BatteryMessage(float batteryLevel)
    : batteryLevel(batteryLevel) {}

void BatteryMessage::toJson(char *buffer, size_t bufferSize)
{
    doc["battery"] = batteryLevel;
    serializeJson(doc, buffer, bufferSize);
}

const char *BatteryMessage::getTopic()
{
    return "esp32/battery";
}

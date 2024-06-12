#include "MqttMessage.h"

BatteryMessage::BatteryMessage(float batteryLevel)
    : batteryLevel(batteryLevel) {}

void BatteryMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = RunID;
    doc["timestamp"] = millis();
    doc["battery"] = batteryLevel;
    serializeJson(doc, buffer, bufferSize);
}

const char *BatteryMessage::getTopic()
{
    return "esp32/battery";
}

MappingMessage::MappingMessage(float x, float y, float theta)
{
    x_coordinate = x;
    y_coordinate = y;
    orientation = theta;
}

void MappingMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = RunID;
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

StatusMessage::StatusMessage() {}

void StatusMessage::toJson(char *buffer, size_t bufferSize)
{
    doc["run_id"] = RunID;
    doc["connection"] = 400;
    serializeJson(doc, buffer, bufferSize);
}

const char *StatusMessage::getTopic()
{
    return "esp32/status";
}

DebugMessage::DebugMessage(const char *message)
    : message(message) {}

void DebugMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = RunID;
    doc["bot_id"] = BotID;
    doc["message"] = message;
    serializeJson(doc, buffer, bufferSize);
}

const char *DebugMessage::getTopic()
{
    return "esp32/debug";
}

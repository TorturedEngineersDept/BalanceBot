#include "MqttMessage.h"

// BatteryMessage Implementation
BatteryMessage::BatteryMessage(float batteryLevel, unsigned long timestamp)
    : MqttMessage(timestamp), batteryLevel(batteryLevel) {}

void BatteryMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = runId;
    doc["timestamp"] = timestamp;
    doc["battery"] = batteryLevel;
    serializeJson(doc, buffer, bufferSize);
}

const char *BatteryMessage::getTopic()
{
    return "esp32/battery";
}

// PowerMessage Implementation
PowerMessage::PowerMessage(float powerLevel, unsigned long timestamp)
    : MqttMessage(timestamp), powerLevel(powerLevel) {}

void PowerMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = runId;
    doc["timestamp"] = timestamp;
    doc["power"] = powerLevel;
    serializeJson(doc, buffer, bufferSize);
}

const char *PowerMessage::getTopic()
{
    return "esp32/power";
}

// MappingMessage Implementation
MappingMessage::MappingMessage(float x, float y, float theta, unsigned long timestamp)
    : MqttMessage(timestamp), x_coordinate(x), y_coordinate(y), orientation(theta) {}

void MappingMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = runId;
    doc["timestamp"] = timestamp;
    doc["x"] = x_coordinate;
    doc["y"] = y_coordinate;
    doc["orientation"] = orientation;
    serializeJson(doc, buffer, bufferSize);
}

const char *MappingMessage::getTopic()
{
    return "esp32/mapping";
}

// StatusMessage Implementation
StatusMessage::StatusMessage(unsigned long timestamp)
    : MqttMessage(timestamp) {}

void StatusMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = runId;
    doc["timestamp"] = timestamp;
    doc["connection"] = 400;
    serializeJson(doc, buffer, bufferSize);
}

const char *StatusMessage::getTopic()
{
    return "esp32/status";
}

// DebugMessage Implementation
DebugMessage::DebugMessage(const char *message, unsigned long timestamp)
    : MqttMessage(timestamp), message(message) {}

void DebugMessage::toJson(char *buffer, size_t bufferSize)
{
    doc.clear();
    doc["run_id"] = runId;
    doc["bot_id"] = BotID;
    doc["timestamp"] = timestamp;
    doc["message"] = message;
    serializeJson(doc, buffer, bufferSize);
}

const char *DebugMessage::getTopic()
{
    return "esp32/debug";
}

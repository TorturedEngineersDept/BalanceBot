#pragma once

#include <ArduinoJson.h>

extern int RunID;
extern int BotID;

class MqttMessage
{
public:
    MqttMessage(unsigned long timestamp) : timestamp(timestamp) {}
    virtual void toJson(char *buffer, size_t bufferSize) = 0;
    virtual const char *getTopic() = 0;

protected:
    int runId = RunID;
    unsigned long timestamp;
};

class BatteryMessage : public MqttMessage
{
public:
    BatteryMessage(float batteryLevel, unsigned long timestamp);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    float batteryLevel;
    StaticJsonDocument<200> doc;
};

class MappingMessage : public MqttMessage
{
public:
    MappingMessage(float x, float y, float theta, unsigned long timestamp);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    float x_coordinate;
    float y_coordinate;
    float orientation;
    StaticJsonDocument<200> doc;
};

class StatusMessage : public MqttMessage
{
public:
    StatusMessage(unsigned long timestamp);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    StaticJsonDocument<200> doc;
};

class DebugMessage : public MqttMessage
{
public:
    DebugMessage(const char *message, unsigned long timestamp);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    const char *message;
    StaticJsonDocument<200> doc;
};

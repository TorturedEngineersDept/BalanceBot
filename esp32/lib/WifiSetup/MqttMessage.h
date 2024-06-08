#pragma once

#include <ArduinoJson.h>

extern int RunID;
extern int BotID;
class MqttMessage
{
public:
    virtual void toJson(char *buffer, size_t bufferSize) = 0;
    virtual const char *getTopic() = 0;

protected:
    int runId = RunID;
};

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

class StatusMessage : public MqttMessage
{
public:
    StatusMessage();
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    StaticJsonDocument<200> doc;
};

class DebugMessage : public MqttMessage
{
public:
    DebugMessage(const char *message);
    void toJson(char *buffer, size_t bufferSize) override;
    const char *getTopic() override;

private:
    const char *message;
    StaticJsonDocument<200> doc;
};

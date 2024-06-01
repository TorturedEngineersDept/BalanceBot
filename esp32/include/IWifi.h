#pragma once

class IWifi
{
public:
    /**
     * Sends a message to the MQTT server, to print debug information.
     * Also prints the message to the serial monitor.
     */
    virtual void print(const char *message) = 0;

    /**
     * Sends a message to the MQTT server, to print debug information.
     * Also prints the message to the serial monitor.
     */
    virtual void println(const char *message) = 0;
};

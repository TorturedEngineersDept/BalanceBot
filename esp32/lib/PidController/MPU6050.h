#pragma once

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "IWifi.h"

class MPU6050
{
public:
    MPU6050();
    bool begin(unsigned long timeout, IWifi &wifi);
    void getEvent(sensors_event_t *a, sensors_event_t *g, sensors_event_t *temp);

private:
    Adafruit_MPU6050 mpu;
};

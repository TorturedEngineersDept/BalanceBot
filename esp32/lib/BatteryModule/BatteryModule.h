#pragma once

#ifndef NATIVE
#include <Arduino.h>
#include <freertos/semphr.h>
#endif
#include <unordered_map>

// Define the pins connected to the PCB
// ADC2 (GPIO 25-27) is used for Wi-Fi, so we can't use it
#define VB_PIN 35 // Analog pin for battery voltage - a0
#define IM_PIN 33 // Analog pin for motor current sense - a1
#define I5_PIN 32 // Analog pin for 5V current sense - a2

/**
 * BatteryModule is responsible for monitoring the battery level.
 */
class BatteryModule
{
public:
    BatteryModule() = delete;

    static int calcBatteryPercentage(float voltage);
    static void setup();
    static void loop();

    // Interface
    static int getBatteryPercentage();
    static float getPowerConsumption();

    static void setBatteryPercentage(int percentage);
    static void setPowerConsumption(float power);

    // Public to allow testing
    static std::unordered_map<float, int> battery_data;

private:
    static float filtered_battery_voltage;
    static int filtered_battery_percentage;

    // Variables to keep track of battery usage
    static float total_power_consumption; // in mW
    static unsigned long start_time;

    /// Constants for voltage dividers and calibration

    // Example ratio based on 100k and 20k resistors
    static constexpr float VOLTAGE_DIVIDER_RATIO = 0.086508; // 0.08170446; // 0.090909
    // Example value in ohms
    static constexpr float CURRENT_SENSE_RESISTOR = 0.01;
    // Maximum voltage for the ADC (Arduino Uno)
    static constexpr float ADC_MAX_VOLTAGE = 3.3;
    // 10-bit ADC resolution
    static constexpr float ADC_RESOLUTION = 3880.0;
    static constexpr float USB_RATIO = 330;
    static constexpr float IM_RATIO = 200;
    // Battery capacity in mAh
    static constexpr float BATTERY_CAPACITY_MAH = 2000.0;

#ifndef NATIVE
    static SemaphoreHandle_t battery_mutex;
    static SemaphoreHandle_t power_mutex;
#endif
};

#include "BatteryModule.h"

std::map<float, int> BatteryModule::battery_data = {
    {16.19, 100},
    {16.18, 100},
    {16.17, 100},
    {16.16, 100},
    {16.15, 100},
    {16.14, 100},
    {16.13, 100},
    {16.12, 100},
    {16.11, 100},
    {16.1, 100},
    {16.09, 100},
    {16.08, 100},
    {16.07, 99},
    {16.06, 99},
    {16.05, 99},
    {16.04, 99},
    {16.03, 99},
    {16.02, 99},
    {16.01, 99},
    {16, 99},
    {15.99, 99},
    {15.98, 99},
    {15.97, 99},
    {15.96, 99},
    {15.95, 99},
    {15.94, 99},
    {15.93, 99},
    {15.92, 99},
    {15.91, 98},
    {15.9, 98},
    {15.89, 98},
    {15.88, 98},
    {15.87, 98},
    {15.86, 98},
    {15.85, 98},
    {15.84, 98},
    {15.83, 98},
    {15.82, 98},
    {15.81, 97},
    {15.8, 97},
    {15.79, 97},
    {15.78, 97},
    {15.77, 97},
    {15.76, 97},
    {15.75, 97},
    {15.74, 97},
    {15.73, 97},
    {15.72, 97},
    {15.71, 97},
    {15.7, 96},
    {15.69, 96},
    {15.68, 96},
    {15.67, 96},
    {15.66, 96},
    {15.65, 96},
    {15.64, 96},
    {15.63, 96},
    {15.62, 95},
    {15.61, 95},
    {15.6, 95},
    {15.59, 95},
    {15.58, 95},
    {15.57, 95},
    {15.56, 95},
    {15.55, 95},
    {15.54, 94},
    {15.53, 94},
    {15.52, 94},
    {15.51, 94},
    {15.5, 94},
    {15.49, 94},
    {15.48, 94},
    {15.47, 93},
    {15.46, 93},
    {15.45, 93},
    {15.44, 93},
    {15.43, 93},
    {15.42, 93},
    {15.41, 92},
    {15.4, 92},
    {15.39, 92},
    {15.38, 92},
    {15.37, 92},
    {15.36, 91},
    {15.35, 91},
    {15.34, 91},
    {15.33, 91},
    {15.32, 90},
    {15.31, 90},
    {15.3, 90},
    {15.29, 90},
    {15.28, 90},
    {15.27, 89},
    {15.26, 89},
    {15.25, 89},
    {15.24, 89},
    {15.23, 88},
    {15.22, 88},
    {15.21, 88},
    {15.2, 87},
    {15.19, 87},
    {15.18, 87},
    {15.17, 86},
    {15.16, 86},
    {15.15, 85},
    {15.14, 85},
    {15.13, 85},
    {15.12, 84},
    {15.11, 83},
    {15.1, 83},
    {15.09, 82},
    {15.08, 81},
    {15.07, 80},
    {15.06, 79},
    {15.05, 77},
    {15.04, 75},
    {15.03, 74},
    {15.02, 70},
    {15.01, 66},
    {15, 63},
    {14.99, 61},
    {14.98, 59},
    {14.97, 57},
    {14.96, 55},
    {14.95, 54},
    {14.94, 52},
    {14.93, 51},
    {14.92, 48},
    {14.91, 47},
    {14.9, 46},
    {14.89, 45},
    {14.88, 44},
    {14.87, 43},
    {14.86, 42},
    {14.85, 42},
    {14.84, 41},
    {14.83, 40},
    {14.82, 39},
    {14.81, 38},
    {14.8, 38},
    {14.79, 37},
    {14.78, 36},
    {14.77, 36},
    {14.76, 35},
    {14.75, 34},
    {14.74, 34},
    {14.73, 33},
    {14.72, 33},
    {14.71, 32},
    {14.7, 31},
    {14.69, 31},
    {14.68, 30},
    {14.67, 30},
    {14.66, 29},
    {14.65, 29},
    {14.64, 28},
    {14.63, 28},
    {14.62, 27},
    {14.61, 27},
    {14.6, 26},
    {14.59, 26},
    {14.58, 25},
    {14.57, 25},
    {14.56, 24},
    {14.55, 24},
    {14.54, 24},
    {14.53, 23},
    {14.52, 23},
    {14.51, 22},
    {14.5, 22},
    {14.49, 21},
    {14.48, 21},
    {14.47, 21},
    {14.46, 20},
    {14.45, 20},
    {14.44, 19},
    {14.43, 19},
    {14.42, 19},
    {14.41, 18},
    {14.4, 18},
    {14.39, 18},
    {14.38, 18},
    {14.37, 17},
    {14.36, 17},
    {14.35, 17},
    {14.34, 16},
    {14.33, 16},
    {14.32, 16},
    {14.31, 15},
    {14.3, 15},
    {14.29, 15},
    {14.28, 14},
    {14.27, 14},
    {14.26, 14},
    {14.25, 14},
    {14.24, 13},
    {14.23, 13},
    {14.22, 13},
    {14.21, 12},
    {14.2, 12},
    {14.19, 12},
    {14.18, 12},
    {14.17, 12},
    {14.16, 11},
    {14.15, 11},
    {14.14, 11},
    {14.13, 11},
    {14.12, 10},
    {14.11, 10},
    {14.1, 10},
    {14.09, 10},
    {14.08, 9},
    {14.07, 9},
    {14.06, 9},
    {14.05, 9},
    {14.04, 9},
    {14.03, 8},
    {14.02, 8},
    {14.01, 8},
    {14, 8},
    {13.99, 8},
    {13.98, 7},
    {13.97, 7},
    {13.96, 7},
    {13.95, 7},
    {13.94, 7},
    {13.93, 6},
    {13.92, 6},
    {13.91, 6},
    {13.9, 6},
    {13.89, 6},
    {13.88, 6},
    {13.87, 5},
    {13.86, 5},
    {13.85, 5},
    {13.84, 5},
    {13.83, 5},
    {13.82, 4},
    {13.81, 4},
    {13.8, 4},
    {13.79, 4},
    {13.78, 4},
    {13.77, 4},
    {13.76, 4},
    {13.75, 3},
    {13.74, 3},
    {13.73, 3},
    {13.72, 3},
    {13.71, 3},
    {13.7, 3},
    {13.69, 2},
    {13.68, 2},
    {13.67, 2},
    {13.66, 2},
    {13.65, 2},
    {13.64, 2},
    {13.63, 2},
    {13.62, 1},
    {13.61, 1},
    {13.6, 1},
    {13.59, 1},
    {13.58, 1},
    {13.57, 1},
    {13.56, 1},
    {13.55, 1},
    {13.54, 0},
    {13.53, 0},
    {13.52, 0},
    {13.51, 0},
    {13.5, 0}};

int BatteryModule::calcBatteryPercentage(float voltage)
{
    int percentage;

    if (voltage > 16.19)
    {
        percentage = 100;
    }
    else if (voltage < 13.5)
    {
        percentage = 0;
    }
    else
    {
        percentage = battery_data[voltage];
    }

    return percentage;
}

#ifndef NATIVE

// Initialize the battery voltage and percentage
SemaphoreHandle_t BatteryModule::battery_mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t BatteryModule::power_mutex = xSemaphoreCreateMutex();
float BatteryModule::filtered_battery_voltage = 17.0;
int BatteryModule::filtered_battery_percentage = 100;
float BatteryModule::total_power_consumption = 0.0;
unsigned long BatteryModule::start_time = millis();

void BatteryModule::setup()
{

    float battery_setup = 0;
    for (int i = 0; i < 200; i++)
    {
        battery_setup += ((analogRead(VB_PIN) / ADC_RESOLUTION) * ADC_MAX_VOLTAGE) / VOLTAGE_DIVIDER_RATIO;
    }
    filtered_battery_voltage = battery_setup / 200;
}

void BatteryModule::loop()
{
    // Read raw ADC values
    int vb_raw = analogRead(VB_PIN);
    int i5_raw = analogRead(I5_PIN);
    int im_raw = analogRead(IM_PIN);

    // Convert raw ADC values to voltages
    float vb_voltage = (vb_raw / ADC_RESOLUTION) * ADC_MAX_VOLTAGE;
    float i5_voltage = ((i5_raw / ADC_RESOLUTION) * ADC_MAX_VOLTAGE);
    float im_voltage = ((im_raw / ADC_RESOLUTION) * ADC_MAX_VOLTAGE);

    /// Calculate actual battery voltage and currents
    float battery_voltage = vb_voltage / VOLTAGE_DIVIDER_RATIO;
    filtered_battery_voltage = 0.01 * (battery_voltage) + (1 - 0.01) * filtered_battery_voltage;
    filtered_battery_voltage = std::round(filtered_battery_voltage * 100.0) / 100.0;
    int battery_percentage = calcBatteryPercentage(filtered_battery_voltage);
    setBatteryPercentage(0.05 * (battery_percentage) + (1 - 0.05) * getBatteryPercentage());

    /// Calculate power consumption
    float current_5v = i5_voltage / (CURRENT_SENSE_RESISTOR * USB_RATIO);
    float current_motor = im_voltage / (CURRENT_SENSE_RESISTOR * IM_RATIO);
    // Power consumption in the 5V rail (in W)
    float power_consumption_5v = 5.25 * current_5v;
    // Power consumption in the motor (in W)
    float power_consumption_motor = battery_voltage * current_motor;
    // Total power consumption (in W)
    setPowerConsumption(power_consumption_5v + power_consumption_motor);

    // Delay before next reading
    delay(1000);
}

int BatteryModule::getBatteryPercentage()
{
    xSemaphoreTake(battery_mutex, portMAX_DELAY);
    int percentage = filtered_battery_percentage;
    xSemaphoreGive(battery_mutex);

    return percentage;
}

float BatteryModule::getPowerConsumption()
{
    xSemaphoreTake(power_mutex, portMAX_DELAY);
    float power = total_power_consumption;
    xSemaphoreGive(power_mutex);

    return power;
}

void BatteryModule::setBatteryPercentage(int percentage)
{
    xSemaphoreTake(battery_mutex, portMAX_DELAY);
    filtered_battery_percentage = percentage;
    xSemaphoreGive(battery_mutex);
}

void BatteryModule::setPowerConsumption(float power)
{
    xSemaphoreTake(power_mutex, portMAX_DELAY);
    total_power_consumption = power;
    xSemaphoreGive(power_mutex);
}

#endif

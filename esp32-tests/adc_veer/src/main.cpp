#include <Arduino.h>

// Define the pins connected to the PCB
#define VB_PIN 34 // Analog pin for battery voltage
#define I5_PIN 35 // Analog pin for 5V current sense
#define IM_PIN 32 // Analog pin for motor current sense

// Constants for voltage dividers and calibration
const float VOLTAGE_DIVIDER_RATIO =
    0.167;                                // Example ratio based on 100k and 20k resistors
const float CURRENT_SENSE_RESISTOR = 0.1; // Example value in ohms
const float ADC_MAX_VOLTAGE = 3.3;        // Maximum voltage for the ADC (ESP32)
const int ADC_RESOLUTION = 4096;          // 12-bit ADC resolution

void setup()
{
    Serial.begin(115200);
    analogReadResolution(12); // Set ADC resolution to 12-bit
}

void loop()
{
    // Read raw ADC values
    int vb_raw = analogRead(VB_PIN);
    int i5_raw = analogRead(I5_PIN);
    int im_raw = analogRead(IM_PIN);

    // Convert raw ADC values to voltages
    float vb_voltage = (vb_raw / (float)ADC_RESOLUTION) * ADC_MAX_VOLTAGE;
    float i5_voltage = (i5_raw / (float)ADC_RESOLUTION) * ADC_MAX_VOLTAGE;
    float im_voltage = (im_raw / (float)ADC_RESOLUTION) * ADC_MAX_VOLTAGE;

    // Calculate actual battery voltage and currents
    float battery_voltage = vb_voltage / VOLTAGE_DIVIDER_RATIO;
    float current_5v = i5_voltage / CURRENT_SENSE_RESISTOR;
    float current_motor = im_voltage / CURRENT_SENSE_RESISTOR;

    // Send the measurements to the Raspberry Pi
    Serial.print("Battery Voltage: ");
    Serial.print(battery_voltage);
    Serial.print(" V, 5V Current: ");
    Serial.print(current_5v);
    Serial.print(" A, Motor Current: ");
    Serial.print(current_motor);
    Serial.println(" A");

    // Delay before next reading
    delay(1000);
}

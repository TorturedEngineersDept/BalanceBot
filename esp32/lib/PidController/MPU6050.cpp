#include "MPU6050.h"

MPU6050::MPU6050()
{
}

bool MPU6050::begin(unsigned long timeout, IWifi &wifi)
{
    unsigned long start = 0;
    wifi.println("Adafruit MPU6050 test!");

    // Try to initialize!
    if (!mpu.begin())
    {
        wifi.println("Failed to find MPU6050 chip");
        while (start < timeout)
        {
            delay(10);
            start += 10;
        }
    }
    if (!mpu.begin())
    {
        return false;
    }

    wifi.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    wifi.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange())
    {
    case MPU6050_RANGE_2_G:
        wifi.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        wifi.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        wifi.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        wifi.println("+-16G");
        break;
    }

    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    wifi.print("Gyro range set to: ");
    switch (mpu.getGyroRange())
    {
    case MPU6050_RANGE_250_DEG:
        wifi.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        wifi.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        wifi.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        wifi.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);
    wifi.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth())
    {
    case MPU6050_BAND_260_HZ:
        wifi.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        wifi.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        wifi.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        wifi.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        wifi.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        wifi.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        wifi.println("5 Hz");
        break;
    }

    // wifi.println("");
    return true;
}

void MPU6050::getEvent(sensors_event_t *a, sensors_event_t *g, sensors_event_t *temp)
{
    mpu.getEvent(a, g, temp);
}

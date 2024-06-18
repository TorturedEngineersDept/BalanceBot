#include <Arduino.h>
#include <PubSubClient.h>
#include <TimerInterrupt_Generic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <step.h>
#include <chrono>
#include <vector>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClient.h>

// MQTT
#define server "18.130.87.186"
#define server_port 1883
#define timeout 5000

// The Stepper pins
#define STEPPER1_DIR_PIN 16  // Arduino D9
#define STEPPER1_STEP_PIN 17 // Arduino D8
#define STEPPER2_DIR_PIN 4   // Arduino D11
#define STEPPER2_STEP_PIN 14 // Arduino D10
#define STEPPER_EN 15        // Arduino D12

// Diagnostic pin for oscilloscope
#define TOGGLE_PIN 32 // Arduino A4

// now all the variables lol
const int PRINT_INTERVAL = 500;
const double LOOP1_INTERVAL = 10;  // inner loop
const double LOOP2_INTERVAL = 200; // outer loop
const double LOOP3_INTERVAL = 20;  // loop to stop/control rotations (work in progress)
const int STEPPER_INTERVAL_US = 20;

double receivedNumber = 0; // number received from webpage
double angle_setpoint = 0; // inner loop setpoint
double speed_setpoint = 0; // outer loop setpoint
double rotation_setpoint = 0;
double rotation_target_angle = 0;
double current_speed_setpoint = 0;
double balancing_setpoint = 0; // setpoint to balance the bot in place
double kp_i;                   // inner
double kd_i = 0;               // inner
double ki_i = 0;               // inner
double kp_o = 0;               // outer
double kd_o = 0;               // outer
double ki_o = 0;               // outer
double comp_filter = 0.98;     // complementary filter coefficient
sensors_event_t a, g, temp;    // MPU sensor
double accXoffset_sum = 0;
double accYoffset_sum = 0;
double accZoffset_sum = 0;
double gyroXoffset_sum = 0;
double gyroYoffset_sum = 0;
double accXoffset;
double accYoffset;
double accZoffset;
double gyroXoffset;
double gyroYoffset;

// MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);
void callback(char *topic, byte *payload, unsigned int length);

// Global objects
ESP32Timer ITimer(3);
Adafruit_MPU6050 mpu; // Default pins for I2C are SCL: IO22/Arduino D3, SDA: IO21/Arduino D4

step step1(STEPPER_INTERVAL_US, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN);
step step2(STEPPER_INTERVAL_US, STEPPER2_STEP_PIN, STEPPER2_DIR_PIN);

// all the webserver stuff
const char *ssid = "hi";
const char *password = "password";

// Interrupt Service Routine for motor update
// Note: ESP32 doesn't support floating point calculations in an ISR
bool TimerHandler(void *timerNo)
{
    static bool toggle = false;

    // Update the stepper motors
    step1.runStepper();
    step2.runStepper();

    // Indicate that the ISR is running
    digitalWrite(TOGGLE_PIN, toggle);
    toggle = !toggle;
    return true;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    pinMode(TOGGLE_PIN, OUTPUT);

    // Try to initialize Accelerometer/Gyroscope
    if (!mpu.begin())
    {
        Serial.println("Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    for (int i = 0; i < 200; i++)
    {
        mpu.getEvent(&a, &g, &temp);
        accXoffset_sum += a.acceleration.x;
        accYoffset_sum += a.acceleration.y;
        accZoffset_sum += a.acceleration.z;
        gyroXoffset_sum += g.gyro.x;
        gyroYoffset_sum += g.gyro.y;
    }

    accXoffset = accXoffset_sum / 200 - 9.81;
    accYoffset = accYoffset_sum / 200;
    accZoffset = accZoffset_sum / 200;
    gyroXoffset = gyroXoffset_sum / 200;
    gyroYoffset = gyroYoffset_sum / 200;

    // Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
    if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, TimerHandler))
    {
        Serial.println("Failed to start stepper interrupt");
        while (1)
            delay(10);
    }
    Serial.println("Initialised Interrupt for Stepper");

    // Set motor acceleration values (initial values, dont really do anything though)
    step1.setAccelerationRad(30);
    step2.setAccelerationRad(30);
    // Enable the stepper motor drivers
    pinMode(STEPPER_EN, OUTPUT);
    digitalWrite(STEPPER_EN, false);

    // START WIFI
    WiFi.begin(ssid, password);
    Serial.println("wifi.begun");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    delay(100);

    // GET WIFI Strength
    Serial.print("WiFi Strength:");
    Serial.println(WiFi.RSSI());
    Serial.println("Check WiFi strength if no connection is established to Broker (<-55)");

    // MQTT SETUP
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // CONNECT TO MQTT BROKER
    client.setServer(server, server_port);
    client.setCallback(callback);

    unsigned long start = 0;
    Serial.print("IP of MQTT Broker: ");
    Serial.println(server);
    Serial.println("Check this IP in case no connection is established to Broker");
    while (!client.connected() && start < timeout)
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("Varun"))
        {
            Serial.println("connected");
            client.subscribe("user/pid");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 1 second");
            delay(1000);
        }
    }
}

void loop()
{

    // Maintain MQTT connection
    if (!client.connected())
    {
        unsigned long start = millis();
        while (!client.connected() && millis() - start < timeout)
        {
            Serial.print("Attempting MQTT reconnection...");
            if (client.connect("ESP32Client"))
            {
                Serial.println("connected");
                client.subscribe("user/pid");
            }
            else
            {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 0.5 seconds");
                delay(500);
            }
        }
    }
    client.loop();

    // Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
    static unsigned long printTimer = 500; // time of the next print
    static unsigned long loop1Timer = 0;   // inner
    static unsigned long loop2Timer = 0;   // outer
    static unsigned long loop3Timer = 0;   // rotation loop
    static unsigned long milliseconds = 0;
    static double acceleration_angle = 0.0;
    static double gyro_angle = 0.0; // current tilt angle
    static double robot_angle = 0.0;
    static double motor_speed1 = 17;
    static double motor_speed2 = 17;

    // variables for rotation loop (work in progress)
    static double rotation = 0, rotation_error = 0, motor_out = 0, rotation_correction = 0, rotation_angle, previous_rotation_error, Pout_r, Dout_r, Iout_r;

    double kp_rotation = 3;
    double kd_rotation = 5;
    double ki_rotation = 0;

    if (millis() > loop3Timer)
    {
        mpu.getEvent(&a, &g, &temp);
        loop3Timer += LOOP3_INTERVAL;
        motor_speed1 = 17;
        motor_speed2 = 17;

        rotation = g.gyro.x - gyroXoffset;

        rotation_angle = rotation * (LOOP3_INTERVAL / 1000) + rotation_angle;

        rotation_target_angle = rotation_target_angle + (rotation_setpoint * 0.4 * LOOP3_INTERVAL) / 1000;

        rotation_error = -(rotation_target_angle - rotation_angle);

        Pout_r = kp_rotation * rotation_error;
        Dout_r = kd_rotation * ((rotation_error - previous_rotation_error) / LOOP3_INTERVAL) * 1000;

        previous_rotation_error = rotation_error;

        rotation_correction = Pout_r + Dout_r;
    }

    // variables for inner loop
    static double angle_error, previous_angle_error = 0, integral_angle = 0, Pout_a = 0, Iout_a = 0, Dout_a = 0, filtered_value = 0;

    double alpha = 0.05, accX, accY, accZ;

    if (millis() > loop1Timer)
    {
        loop1Timer += LOOP1_INTERVAL;
        mpu.getEvent(&a, &g, &temp);
        accX = a.acceleration.x - accXoffset;
        accY = a.acceleration.y - accYoffset;
        accZ = a.acceleration.z - accZoffset;

        // Calculate Tilt using accelerometer and sin x = x approximation for a small tilt angle
        // acceleration_angle = (accZ) / (9.67);
        acceleration_angle = (accZ) / sqrt(pow(accY, 2) + pow(accX, 2));

        gyro_angle = g.gyro.y - gyroYoffset;

        robot_angle = (1 - comp_filter) * (acceleration_angle) + comp_filter * ((gyro_angle * (LOOP1_INTERVAL / 1000)) + robot_angle);

        // PIDeez Nuts

        angle_error = angle_setpoint - robot_angle * 57.32;

        Pout_a = kp_i * angle_error;

        integral_angle = integral_angle + ((angle_error * LOOP1_INTERVAL) / 1000);
        Iout_a = ki_i * integral_angle;

        Dout_a = kd_i * ((angle_error - previous_angle_error) / LOOP1_INTERVAL) * 1000;

        motor_out = Pout_a + Iout_a + Dout_a;

        previous_angle_error = angle_error;

        if (motor_out >= 0)
        {
            step1.setTargetSpeedRad(motor_speed1);
            step2.setTargetSpeedRad(-motor_speed2);
        }
        else
        {
            step1.setTargetSpeedRad(-motor_speed1);
            step2.setTargetSpeedRad(motor_speed2);
        }

        step1.setAccelerationRad(motor_out + rotation_correction); // uses absolute values
        step2.setAccelerationRad(motor_out - rotation_correction);

        filtered_value = alpha * ((step1.getSpeedRad() - step2.getSpeedRad()) / 2) + (1 - alpha) * filtered_value;
    }

    // variables for outer loop
    static double avgspeed = 0, avg_temp = 0, speed_error = 0, previous_speed_error = 0, integral_speed = 0, derivative_speed = 0, Pout_s = 0, Iout_s = 0, Dout_s = 0, ramp_rate = 0;

    double max_ramp_rate = 0.5;
    double time_constant = 2000;
    double beta = 1 - exp(-LOOP2_INTERVAL / time_constant);

    static int speed_size = 0;

    if (millis() > loop2Timer)
    {
        loop2Timer += LOOP2_INTERVAL;

        // ramp_rate = max_ramp_rate * fabs(speed_error); // Variable ramp rate proportional to the error

        //     // Clamp ramp_rate to max_ramp_rate
        //     if (ramp_rate > max_ramp_rate) {
        //         ramp_rate = max_ramp_rate;
        //     }

        //     if (fabs(speed_error) < ramp_rate) {
        //         // If the error is smaller than the ramp rate, set it directly
        //         current_speed_setpoint = speed_setpoint;
        //     } else if (current_speed_setpoint < speed_setpoint) {
        //         // Increase the setpoint gradually
        //         current_speed_setpoint += ramp_rate;
        //     } else if (current_speed_setpoint > speed_setpoint) {
        //         // Decrease the setpoint gradually
        //         current_speed_setpoint -= ramp_rate;
        //     }

        current_speed_setpoint += beta * (speed_setpoint - current_speed_setpoint);

        // filtered_value = (step1.getSpeedRad()-step2.getSpeedRad())/2;
        speed_error = -(current_speed_setpoint - filtered_value);

        Pout_s = kp_o * speed_error;

        integral_speed = integral_speed + speed_error * (LOOP2_INTERVAL / 1000);

        if (ki_o == 0)
        {
            integral_speed = 0;
        }

        Iout_s = ki_o * integral_speed;

        derivative_speed = ((speed_error - previous_speed_error) / LOOP2_INTERVAL) * 1000;

        Dout_s = kd_o * derivative_speed;

        angle_setpoint = Pout_s + Iout_s + Dout_s + balancing_setpoint;

        if (speed_setpoint > 0)
        {
            if (angle_setpoint > balancing_setpoint + 1)
            {
                angle_setpoint = balancing_setpoint + 1;
            }
            else if (angle_setpoint < balancing_setpoint - 5)
            {
                angle_setpoint = balancing_setpoint - 5;
            }
        }

        else if (speed_setpoint < 0)
        {
            if (angle_setpoint < balancing_setpoint - 1)
            {
                angle_setpoint = balancing_setpoint - 1;
            }
            else if (angle_setpoint > balancing_setpoint + 5)
            {
                angle_setpoint = balancing_setpoint + 5;
            }
        }

        previous_speed_error = speed_error;
    }

    if (millis() > printTimer)
    {
        printTimer += PRINT_INTERVAL;

        Serial.print(rotation_setpoint * 57.32);
        Serial.print(' ');
        Serial.println(rotation_angle * 57.32);
        // Serial.print(' ');
        // Serial.println(filtered_value);

        // Publish the data to the MQTT broker
        StaticJsonDocument<200> doc;
        doc["timestamp"] = millis();
        doc["tilt"] = rotation_angle;
        // doc["velocity"] = filtered_value;

        char buffer[512];
        serializeJson(doc, buffer);
        // client.publish("tuning/graphing", buffer);
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Create a buffer to store the payload
    char payloadStr[length + 1];
    memcpy(payloadStr, payload, length);
    payloadStr[length] = '\0'; // Null-terminate the string

    Serial.println(payloadStr);

    // Parse the JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payloadStr);

    if (error)
    {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
    }

    if (strcmp(topic, "user/pid") == 0)
    {
        // Extract values from the JSON document
        kp_i = doc["kp_i"].as<double>();
        ki_i = doc["ki_i"].as<double>();
        kd_i = doc["kd_i"].as<double>();
        balancing_setpoint = doc["setpoint_i"].as<double>();
        kp_o = doc["kp_o"].as<double>();
        ki_o = doc["ki_o"].as<double>();
        kd_o = doc["kd_o"].as<double>();
        speed_setpoint = doc["setpoint_o"].as<double>();
        rotation_setpoint = doc["rotation_setpoint"].as<double>();
        Serial.println("kp_i: " + String(kp_i) +
                       ", ki_i: " + String(ki_i) +
                       ", kd_i: " + String(kd_i) +
                       ", Balancing Setpoint: " + String(balancing_setpoint) +
                       ", kp_o: " + String(kp_o) +
                       ", ki_o: " + String(ki_o) +
                       ", kd_o: " + String(kd_o) +
                       ", Speed Setpoint: " + String(speed_setpoint));
    }
    else
    {
        Serial.println("Unsupported topic");
    }
}

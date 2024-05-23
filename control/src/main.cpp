#include <Arduino.h>
#include <TimerInterrupt_Generic.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <step.h>
#include <chrono>

// The Stepper pins
#define STEPPER1_DIR_PIN 16   //Arduino D9
#define STEPPER1_STEP_PIN 17  //Arduino D8
#define STEPPER2_DIR_PIN 4    //Arduino D11
#define STEPPER2_STEP_PIN 14  //Arduino D10
#define STEPPER_EN 15         //Arduino D12

// Diagnostic pin for oscilloscope
#define TOGGLE_PIN  32        //Arduino A4

const int PRINT_INTERVAL = 50;
const double LOOP_INTERVAL = 5;
const int  STEPPER_INTERVAL_US = 10;

//Global objects
ESP32Timer ITimer(3);
Adafruit_MPU6050 mpu;         //Default pins for I2C are SCL: IO22/Arduino D3, SDA: IO21/Arduino D4

step step1(STEPPER_INTERVAL_US,STEPPER1_STEP_PIN,STEPPER1_DIR_PIN );
step step2(STEPPER_INTERVAL_US,STEPPER2_STEP_PIN,STEPPER2_DIR_PIN );


//Interrupt Service Routine for motor update
//Note: ESP32 doesn't support floating point calculations in an ISR
bool TimerHandler(void * timerNo)
{
  static bool toggle = false;

  //Update the stepper motors
  step1.runStepper();
  step2.runStepper();

  //Indicate that the ISR is running
  digitalWrite(TOGGLE_PIN,toggle);  
  toggle = !toggle;
	return true;
}

void setup()
{
  Serial.begin(115200);
  pinMode(TOGGLE_PIN,OUTPUT);

  // Try to initialize Accelerometer/Gyroscope
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  //Attach motor update ISR to timer to run every STEPPER_INTERVAL_US μs
  if (!ITimer.attachInterruptInterval(STEPPER_INTERVAL_US, TimerHandler)) {
    Serial.println("Failed to start stepper interrupt");
    while (1) delay(10);
    }
  Serial.println("Initialised Interrupt for Stepper");

  //Set motor acceleration values
  step1.setAccelerationRad(30.0);
  step2.setAccelerationRad(30.0);

  //Enable the stepper motor drivers
  pinMode(STEPPER_EN,OUTPUT);
  digitalWrite(STEPPER_EN, false);

}

void loop()
{
  //Static variables are initialised once and then the value is remembered betweeen subsequent calls to this function
  static unsigned long printTimer = 0;  //time of the next print
  static unsigned long loopTimer = 0;   //time of the next control update
  static double tiltx = 0.0;
  static double gyrox = 0.0;             //current tilt angle
  static double gyroangle = 0.0;
  static double theta_n = 0.0;
  
 
  float kp = 2;
  float ki = 0.002;  //potentially don't need if we get the setpoint right (correct offset calibration)
  float kd = 1;
  float setpoint = -0.51;
  double error, previous_error = 0, integral = 0, derivative = 0, previous_derivative = 0, Pout, Iout, Dout, motor_out;

  //Run the control loop every LOOP_INTERVAL ms
  if (millis() > loopTimer) {
    loopTimer += LOOP_INTERVAL;

    // Fetch data from MPU6050
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    //Calculate Tilt using accelerometer and sin x = x approximation for a small tilt angle
    tiltx = asin(a.acceleration.z/9.81)-0.05;

    gyrox = g.gyro.y - 0.04;   
    gyroangle = gyroangle + (gyrox*(LOOP_INTERVAL));

    theta_n = 0.04*(tiltx*100) + 0.96*((gyrox*LOOP_INTERVAL)/10 + theta_n);

    //PIDeez Nuts


    error = setpoint - theta_n;

    Pout = kp*error;

    integral = integral + error*(LOOP_INTERVAL/1000);
    Iout = ki*integral;

    derivative = ((error - previous_error)/LOOP_INTERVAL)*1000;
    derivative = previous_derivative + 0.2*(derivative-previous_derivative); 
    previous_derivative = derivative;

    Dout = kd*derivative;

    motor_out = Pout + Iout + Dout;

    previous_error = error;

    //Set target motor speed

    step1.setTargetSpeedRad(motor_out);
    step2.setTargetSpeedRad(-motor_out);
  }
  
  //Print updates every PRINT_INTERVAL ms
  // if (millis() > printTimer) {
  //   printTimer += PRINT_INTERVAL;
    
  //   Serial.print(error);
  //   Serial.print(' ');
  //   Serial.print(motor_out);
  //   Serial.println();
    
  //   // Serial.print(tiltx*100);
  //   // Serial.print(' ');
  //   // Serial.print(step1.getSpeedRad());
  //   // Serial.print(' ');
  //   // Serial.print(gyroangle/10);
  //   // Serial.print(' ');
  //   // Serial.print(theta_n);
  //   // Serial.println();
  // }
  
}
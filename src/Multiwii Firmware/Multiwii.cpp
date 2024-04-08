#include <Arduino.h>
#include <PPMReader.h>
#include <Simple_MPU6050.h>
#include <PID_v1.h>

// Declare Pin
#define PPM_PIN 19
#define CHANNEL_AMOUNT 6

#define ESC_PWM_PIN_1 3
#define ESC_PWM_PIN_2 5
#define ESC_PWM_PIN_3 6
#define ESC_PWM_PIN_4 7

// Declare Variable
uint32_t currMill, prevMill;
const uint16_t telemetryInterval = 100;

int ESCVal_1, ESCVal_2, ESCVal_3, ESCVal_4;
uint16_t PPMVal[6];

Quaternion quaternion;
VectorFloat gravity;
VectorInt16 aa, aaReal, aaWorld;
float ypr[3];

double SP_Pitch, SP_Roll, SP_Yaw, SP_Throttle;
double IN_Pitch, IN_Roll, IN_Yaw, IN_Throttle;
double OUT_Pitch, OUT_Roll, OUT_Yaw, OUT_Throttle;
double Kp = 2, Ki = 0.5, Kd = 10;

// Declare Object
PPMReader ppm(PPM_PIN, CHANNEL_AMOUNT);
Simple_MPU6050 mpu;
PID pitchPID(&IN_Pitch, &OUT_Pitch, &SP_Pitch, Kp, Ki, Kd, DIRECT);
PID rollPID(&IN_Roll, &OUT_Roll, &SP_Roll, Kp, Ki, Kd, DIRECT);

// Functions
void sensorReading(int16_t *gyro, int16_t* accel, int32_t* quat) {
  mpu.GetQuaternion(&quaternion, quat);
  mpu.GetGravity(&gravity, &quaternion);
  mpu.GetYawPitchRoll(ypr, &quaternion, &gravity);
  mpu.SetAccel(&aa, accel);
  mpu.GetLinearAccel(&aaReal, &aa, &gravity);
  mpu.GetLinearAccelInWorld(&aaWorld, &aaReal, &quaternion);
  
  ypr[0] *= 180 / M_PI;
  ypr[1] *= 180 / M_PI;
  ypr[2] *= 180 / M_PI;
  
  if(ypr[0] < 0) {
    ypr[0] += 360;
  }
  if(ypr[1] > 90) {
    ypr[1] = 180 - ypr[1];
  }
  if(ypr[1] < -90) {
    ypr[1] = -180 - ypr[1];
  }
}

void setup() {
  // Serial Initialization
  Serial.begin(115200);
  Serial2.begin(57600);
  
  // I2C Initialization
  Wire.begin();
  Wire.setClock(400000);

  // Pin Mode Setup
  pinMode(ESC_PWM_PIN_1, OUTPUT);
  pinMode(ESC_PWM_PIN_2, OUTPUT);
  pinMode(ESC_PWM_PIN_3, OUTPUT);
  pinMode(ESC_PWM_PIN_4, OUTPUT);

  // ESC Initial Value
  analogWrite(ESC_PWM_PIN_1, 127);
  analogWrite(ESC_PWM_PIN_2, 127);
  analogWrite(ESC_PWM_PIN_3, 127);
  analogWrite(ESC_PWM_PIN_4, 127);
  delay(5000);

  // MPU6050 Initialization
  mpu.begin();
  mpu.Set_DMP_Output_Rate_Hz(100);
  mpu.SetAddress(0x68);
  mpu.CalibrateMPU();
  mpu.load_DMP_Image();
  mpu.on_FIFO(sensorReading);

  // PID Initialization
  pitchPID.SetMode(AUTOMATIC);
  pitchPID.SetOutputLimits(-55, 55);
  rollPID.SetMode(AUTOMATIC);
  rollPID.SetOutputLimits(-55, 55);
}

void loop() {
  // Read MPU6050
  mpu.dmp_read_fifo(false);

  // Assign PPM Value
  for(uint8_t channel = 1; channel <= CHANNEL_AMOUNT; channel++) {
    PPMVal[channel-1] = ppm.latestValidChannelValue(channel, 0);
  }

  ESCVal_1 = map(PPMVal[2], 1000, 2000, 140, 250);
  ESCVal_2 = map(PPMVal[2], 1000, 2000, 140, 250);
  ESCVal_3 = map(PPMVal[2], 1000, 2000, 140, 250);
  ESCVal_4 = map(PPMVal[2], 1000, 2000, 140, 250);

  SP_Pitch = map(PPMVal[1], 1000, 2000, 10, -10);
  IN_Pitch = ypr[1];
  pitchPID.Compute();
  ESCVal_1 += OUT_Pitch;
  ESCVal_2 += OUT_Pitch;
  ESCVal_3 -= OUT_Pitch;
  ESCVal_4 -= OUT_Pitch;

  SP_Roll = map(PPMVal[0], 1000, 2000, 10, -10);
  IN_Roll = -ypr[2];
  rollPID.Compute();
  ESCVal_1 -= OUT_Roll;
  ESCVal_2 += OUT_Roll;
  ESCVal_3 -= OUT_Roll;
  ESCVal_4 += OUT_Roll;

/*
  // Throttle
  ESCVal_1 = map(PPMVal[2], 1000, 2000, 140, 250);
  ESCVal_2 = map(PPMVal[2], 1000, 2000, 140, 250);
  ESCVal_3 = map(PPMVal[2], 1000, 2000, 140, 250);
  ESCVal_4 = map(PPMVal[2], 1000, 2000, 140, 250);

  // Pitch
  if(PPMVal[1] > 1550 || PPMVal[1] < 1450) {
    ESCVal_1 -= map(PPMVal[1], 1000, 2000, -32, 32);
    ESCVal_2 -= map(PPMVal[1], 1000, 2000, -32, 32);
    ESCVal_3 += map(PPMVal[1], 1000, 2000, -32, 32);
    ESCVal_4 += map(PPMVal[1], 1000, 2000, -32, 32);
  }

  // Roll
  if(PPMVal[0] > 1550 || PPMVal[0] < 1450) {
    ESCVal_1 += map(PPMVal[0], 1000, 2000, -32, 32);
    ESCVal_2 -= map(PPMVal[0], 1000, 2000, -32, 32);
    ESCVal_3 += map(PPMVal[0], 1000, 2000, -32, 32);
    ESCVal_4 -= map(PPMVal[0], 1000, 2000, -32, 32);
  }

  // Yaw
  if(PPMVal[3] > 1550 || PPMVal[3] < 1450) {
    ESCVal_1 += map(PPMVal[3], 1000, 2000, -32, 32);
    ESCVal_2 -= map(PPMVal[3], 1000, 2000, -32, 32);
    ESCVal_3 -= map(PPMVal[3], 1000, 2000, -32, 32);
    ESCVal_4 += map(PPMVal[3], 1000, 2000, -32, 32);
  }
*/
  // PWM Limiter
  if(ESCVal_1 > 250) {
    ESCVal_1 = 250;
  } else if(ESCVal_1 < 140) {
    ESCVal_1 = 140;
  }
  if(ESCVal_2 > 250) {
    ESCVal_2 = 250;
  } else if(ESCVal_2 < 140) {
    ESCVal_2 = 140;
  }
  if(ESCVal_3 > 250) {
    ESCVal_3 = 250;
  } else if(ESCVal_3 < 140) {
    ESCVal_3 = 140;
  }
  if(ESCVal_4 > 250) {
    ESCVal_4 = 250;
  } else if(ESCVal_4 < 140) {
    ESCVal_4 = 140;
  }

  // Write ESC
  analogWrite(ESC_PWM_PIN_1, ESCVal_1);
  analogWrite(ESC_PWM_PIN_2, ESCVal_2);
  analogWrite(ESC_PWM_PIN_3, ESCVal_3);
  analogWrite(ESC_PWM_PIN_4, ESCVal_4);
  
  // Serial Print
  char buffer[64];
  sprintf(buffer, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d", int(IN_Pitch), int(OUT_Pitch), int(SP_Pitch), int(ypr[1]), int(ypr[2]), int(ypr[0]), ESCVal_1, ESCVal_2, ESCVal_3, ESCVal_4);
  Serial.println(buffer);

  // Send Telemetry
  currMill = millis();
  if(currMill - prevMill >= telemetryInterval) {
    prevMill = currMill;
    char telemetry[64];
    sprintf(telemetry, "%d;%d;%d;%d;%d;%d;%d;\r\n", int(ypr[1]), int(ypr[2]), int(ypr[0]), ESCVal_1, ESCVal_2, ESCVal_3, ESCVal_4);
    Serial2.print(telemetry);
  }
}
// Inisialisasi Library ------------------------------------
#include <Wire.h>
#include <Arduino.h>
#include <MPU6050_tockn.h>

// Inisialisasi MPU6050 ------------------------------------
TwoWire I2C_Wire = Wire;  // Inisialisasi objek TwoWire
MPU6050 MPU(I2C_Wire);  // Inisialisasi objek MPU6050 menggunakan konstruktor pertama

// Inisialisasi Pin PWM ESC ---------------------------------
#define prop1 3   //    1     3
#define prop2 5   //       ^
#define prop3 6   //    4     2
#define prop4 7   // Konfigurasi Propeler (^ = Heading Y)
#define remote 8  // Pin PWM Remot
#define disarm 9  // Pin PWM Switch Remot

// Inisialisasi Variable Data MPU6050 -----------------------
float Gyro_angle_x, Gyro_angle_y, Gyro_angle_z;
int throttle;

// Inisialisaisi Kontrol PID --------------------------------
float setRoll = 0, setPitch = 0, setYaw = 0; // Sudut yang diinginkan
float kp = 0.7;    // 0.7
float ki = 0.006;  // 0.006
float kd = 1.2;    // 1.2

// Inisialisasi Variabel Kontrol PID ------------------------
unsigned long last_time = 0;
double time_interval = 0.05; // Waktu interval (ms)

double last_roll_error  = 0, roll_P  = 0, roll_I  = 0, roll_D  = 0;
double last_pitch_error = 0, pitch_P = 0, pitch_I = 0, pitch_D = 0;

float roll_PID, pitch_PID;
float roll_error, pitch_error;
float roll_previous_error = 0, pitch_previous_error = 0;

void setup() {
  Wire.begin();
  Serial.begin(57600);
  pinMode(remote, INPUT);
  pinMode(disarm, INPUT_PULLUP);

  pinMode(prop1, OUTPUT); // Motor kiri depan
  pinMode(prop2, OUTPUT); // Motor kanan belakang
  pinMode(prop3, OUTPUT); // Motor kanan depan
  pinMode(prop4, OUTPUT); // Motor kiri belakang

  MPU.begin(); // Inisialisasi MPU6050
  // MPU.calcGyroOffsets(true); // Kalibrasi sensor MPU6050, lakukan saat sensor dalam keadaan diam
  delay(2000);

  // Membaca data MPU6050
  MPU.update();
  Gyro_angle_x = MPU.getAngleX();
  Gyro_angle_y = MPU.getAngleY();
  Gyro_angle_z = MPU.getAngleZ();

  // Setup BLDC
  analogWrite(prop1, 150);
  analogWrite(prop2, 175);
  analogWrite(prop3, 150);
  analogWrite(prop4, 150);
  Serial.println(" ");
  // Serial.println("Setup ON");
  delay(1000);

  analogWrite(prop1, 0);
  analogWrite(prop2, 0);
  analogWrite(prop3, 0);
  analogWrite(prop4, 0);
  // Serial.println("Setup OFF");

  delay(5000);
}

void loop() {
  int ch3 = pulseIn(remote, HIGH);
  int read = digitalRead(disarm);
  int set_pwm = 0;
  
  if (read == LOW){
    analogWrite(prop1, 0);
    analogWrite(prop4, 0);
    analogWrite(prop2, 0);
    analogWrite(prop3, 0);
    Serial.print("Disarm");
  }

  else if (read == HIGH){
    Serial.print("Arm");

    if (ch3 < 1100){
      analogWrite(prop1, 0);
      analogWrite(prop4, 0);
      analogWrite(prop2, 0);
      analogWrite(prop3, 0);
    }
  
    else if (ch3 >= 1100){
      //int set_pwm = 200;  // Set Ke 150 - 250;
      set_pwm = map(ch3, 970, 1980, 130, 254);
      throttle = map(set_pwm, 130, 254, 0, 100);

      // Membaca data MPU6050 --------------------
      MPU.update();
      Gyro_angle_x = MPU.getAngleX();
      Gyro_angle_y = MPU.getAngleY();
      Gyro_angle_z = MPU.getAngleZ();

      // Hitung Error ----------------------------
      float roll_er = Gyro_angle_x - setRoll;
      float pitch_er = Gyro_angle_y - setPitch;

      roll_error = abs(roll_er);
      pitch_error = abs(pitch_er);

      // Mendefinisikan Waktu --------------------
      unsigned long current_time = millis();
      double dt = (current_time - last_time) / 1000.0; // Waktu dalam detik

      // Perhitungan PID -------------------------
      roll_D = kd * ((roll_error - last_roll_error) / dt);
      roll_I = ki * (roll_I + (roll_error * dt));
      roll_P = kp * roll_error;

      pitch_D = kd * ((pitch_error - last_pitch_error) / dt);
      pitch_I = ki * (pitch_I + (pitch_error * dt));
      pitch_P = kp * pitch_error;
  
      float rollPID = roll_P + roll_I + roll_D;
      float pitchPID = pitch_P + pitch_I + pitch_D;
      roll_PID = abs(rollPID);
      pitch_PID = abs(pitchPID);

      // Menyimpan Data Terakhir -------------------
      last_roll_error = roll_error;
      last_pitch_error = pitch_error;
      last_time = current_time;

      // Inisialisasi Hasil PID Dengan Sinyal PWM
      int ro_pwm = set_pwm + roll_PID;
      int pi_pwm = set_pwm + pitch_PID;

      int inv_ro = set_pwm - roll_PID;
      int inv_pi = set_pwm - pitch_PID;

      int roll_pwm = constrain(ro_pwm, 150, 254);
      int pitch_pwm = constrain(pi_pwm, 150, 254);

      int inv_roll_pwm = constrain(inv_ro, 150, 254);
      int inv_pitch_pwm = constrain(inv_pi, 150, 254);


      // Kontrol Roll -------------------------------
      if (Gyro_angle_x > setRoll) {
        analogWrite(prop1, inv_roll_pwm);
        analogWrite(prop3, inv_roll_pwm);
        analogWrite(prop2, roll_pwm);
        analogWrite(prop4, roll_pwm);
        /*
        Serial.print(Gyro_angle_x);
        Serial.print(" ");
        Serial.print(roll_pwm);
        Serial.print(" ");
        Serial.print(inv_roll_pwm);
        Serial.print(" 2 4 ");
        */
      }

      else if (Gyro_angle_x < setRoll) {
        analogWrite(prop1, roll_pwm);
        analogWrite(prop3, roll_pwm);
        analogWrite(prop2, inv_roll_pwm);
        analogWrite(prop4, inv_roll_pwm);
        /*
        Serial.print(Gyro_angle_x);
        Serial.print(" ");
        Serial.print(roll_pwm);
        Serial.print(" ");
        Serial.print(inv_roll_pwm);
        Serial.print(" 1 3 ");
        */
      }

      // Serial.print(" | ");

      // Kontrol Pitch ------------------------------
      if (Gyro_angle_y > setPitch) {
        analogWrite(prop1, inv_pitch_pwm);
        analogWrite(prop4, inv_pitch_pwm);
        analogWrite(prop2, pitch_pwm);
        analogWrite(prop3, pitch_pwm);
        /*
        Serial.print(Gyro_angle_x);
        Serial.print(" ");
        Serial.print(pitch_pwm);
        Serial.print(" ");
        Serial.print(inv_pitch_pwm);
        Serial.print(" 2 3 ");
        */
      }

      else if (Gyro_angle_y < setPitch) {
        analogWrite(prop1, pitch_pwm);
        analogWrite(prop4, pitch_pwm);
        analogWrite(prop2, inv_pitch_pwm);
        analogWrite(prop3, inv_pitch_pwm);
        /*
        Serial.print(Gyro_angle_y);
        Serial.print(" ");
        Serial.print(pitch_pwm);
        Serial.print(" ");
        Serial.print(inv_pitch_pwm);
        Serial.print(" 1 4 ");
        */
      }
    }

    Serial.print(throttle);
    Serial.print(" ");

    Serial.print(Gyro_angle_x);
    Serial.print(" ");

    Serial.print(Gyro_angle_y);
    Serial.print(" ");

    Serial.print(Gyro_angle_z);
    Serial.print(" ");

    Serial.print(roll_PID);
    Serial.print(" ");

    Serial.print(pitch_PID);
  }

  Serial.println(" ");

  // Tambahkan delay jika diperlukan untuk kontrol PID yang lebih tepat
  delay(40);
}

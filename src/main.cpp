#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ================= PCA9685 =================
#define SERVO_FREQ 50

#define SERVOMIN 125
#define SERVOMAX 725

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// ================= ESP32 I2C =================
#define SDA_PIN 21
#define SCL_PIN 22

// ================= POTENTIOMETER =================
int potWrist    = 32;
int potElbow    = 35;
int potShoulder = 34;
int potBase     = 39;

// ================= SERVO CHANNEL =================
int hand     = 11;
int wrist    = 12;
int elbow    = 13;
int shoulder = 14;
int base     = 15;

// ================= BUTTON =================
int buttonPin = 33;

// ================= FILTER =================
// 0.0 = smooth มาก
// 1.0 = ตอบสนองไว
// Kalman filter tuning:
// Increase processNoise for faster response.
// Increase errorMeasure for smoother output.

// ================= STRUCT =================
struct JointState
{
  int potVal;
  int angle;
  int pulse;
};

struct KalmanFilter
{
  float estimate;
  float errorEstimate;
  float errorMeasure;
  float processNoise;
};

KalmanFilter wristFilter    = {0, 1, 25, 0.4};
KalmanFilter elbowFilter    = {0, 1, 25, 0.4};
KalmanFilter shoulderFilter = {0, 1, 25, 0.4};
KalmanFilter baseFilter     = {0, 1, 25, 0.4};

const unsigned long SERIAL_PRINT_INTERVAL_MS = 250;
unsigned long lastSerialPrint = 0;

// ======================================================
// Convert Angle -> PCA9685 Pulse
// ======================================================
int angleToPulse(int angle)
{
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

// ======================================================
// KALMAN FILTER
// ======================================================
float filterAnalog(float raw, KalmanFilter &filter)
{
  filter.errorEstimate += filter.processNoise;

  float kalmanGain = filter.errorEstimate / (filter.errorEstimate + filter.errorMeasure);
  filter.estimate = filter.estimate + kalmanGain * (raw - filter.estimate);
  filter.errorEstimate = (1.0 - kalmanGain) * filter.errorEstimate;

  return filter.estimate;
}

// ======================================================
// Move Servo from Potentiometer
// ======================================================
JointState moveMotor(int controlIn, int motorOut, KalmanFilter &filter, bool reversed = false)
{
  int raw = analogRead(controlIn);

  // FILTER
  int potVal = (int)filterAnalog(raw, filter);

  // map ADC -> angle
  int angle = map(potVal, 0, 4095, 0, 180);

  if(reversed)
  {
    angle = 180 - angle;
  }

  angle = constrain(angle, 0, 180);

  int pulse = angleToPulse(angle);

  pwm.setPWM(motorOut, 0, pulse);

  return {potVal, angle, pulse};
}

// ======================================================
// SETUP
// ======================================================
void setup()
{
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(potWrist, INPUT);
  pinMode(potElbow, INPUT);
  pinMode(potShoulder, INPUT);
  pinMode(potBase, INPUT);

  // initialize filter
  wristFilter.estimate    = analogRead(potWrist);
  elbowFilter.estimate    = analogRead(potElbow);
  shoulderFilter.estimate = analogRead(potShoulder);
  baseFilter.estimate     = analogRead(potBase);

  pwm.setPWM(hand, 0, angleToPulse(90));

  Serial.println("ESP32 Robot Arm Ready");
}

// ======================================================
// LOOP
// ======================================================
void loop()
{
  JointState wristState =
      moveMotor(potWrist, wrist, wristFilter, true);

  JointState elbowState =
      moveMotor(potElbow, elbow, elbowFilter);

  JointState shoulderState =
      moveMotor(potShoulder, shoulder, shoulderFilter);

  JointState baseState =
      moveMotor(potBase, base, baseFilter);

  // ================= HAND CONTROL =================
  int pushButton = digitalRead(buttonPin);

  if(pushButton == LOW)
  {
    pwm.setPWM(hand, 0, angleToPulse(135));
  }
  else
  {
    pwm.setPWM(hand, 0, angleToPulse(90));
  }

  // ================= SERIAL =================
  if(millis() - lastSerialPrint >= SERIAL_PRINT_INTERVAL_MS)
  {
    lastSerialPrint = millis();

    Serial.print("Wrist=");
    Serial.print(wristState.angle);

    Serial.print(" | Elbow=");
    Serial.print(elbowState.angle);

    Serial.print(" | Shoulder=");
    Serial.print(shoulderState.angle);

    Serial.print(" | Base=");
    Serial.print(baseState.angle);

    Serial.print(" | Hand=");
    Serial.println(pushButton == LOW ? "OPEN" : "CLOSE");
  }

  delay(10);
}

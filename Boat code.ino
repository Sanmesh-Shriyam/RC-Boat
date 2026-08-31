

// Include required libraries
#include <Ps3Controller.h>
#include <ESP32Servo.h>

// Create servo object
Servo srvmtr;

// Controller Joystick Values
int rightX;
int rightY;

// Servo Position
int servoPos = 90;

// Servo Pin
#define SERVO_PIN 13
const int in1 = 2; // Motor 1 direction pin
const int in2 = 4; // Motor 1 direction pin
const int enA = 15; // Motor 1 enable pin

const int motorFreq = 1000;
const int motorResolution = 8;

const int motorAChannel = 3;
int motorAPWM = 0;

bool motorDir = true;

int leftY = 0;
// Callback Function
void notify() {

  // Get Joystick value
  rightX = (Ps3.data.analog.stick.rx);
  rightY = (Ps3.data.analog.stick.ry);
  leftY = (Ps3.data.analog.stick.ly);
  if (leftY < 0) {
    // Direction is forward
    motorDir = true;
  } else {
    // Direction is reverse
    motorDir = false;
  }
  int speedY = (abs(leftY) * 2);
  motorAPWM = speedY;
  motorAPWM = constrain(motorAPWM, 0, 255);
  // Check if joystick was moved upward, indicating return to home (90 degrees)
  if (rightY < -100) {
    servoPos = 90;
    srvmtr.write(servoPos);
    delay(10);
  } else {

    // See if joystick was moved right or right, and in what direction. If moved, move servo in that direction
    if (rightX < -10 && servoPos < 150) {
      servoPos++;
      srvmtr.write(servoPos);
      delay(10);
    }
    if (rightX > 10 && servoPos > 45) {
      servoPos--;
      srvmtr.write(servoPos);
      delay(10);
    }
  }

  if(motorDir == true)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, motorAPWM); // Full speed
    //delay(2);
  }
  else
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enA, motorAPWM); // Full speed
  delay(2);
  }

  // Print to Serial Monitor
  Serial.print("X value = ");
  Serial.print(rightX);
  Serial.print(" - Y value = ");
  Serial.print(rightY);
  Serial.print(" - Servo Pos = ");
  Serial.println(servoPos);
}

// On Connection function
void onConnect() {
  // Print to Serial Monitor
  Serial.println("Connected.");
}

void setup() {

  // Setup Serial Monitor for testing
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);

  // Define Callback Function
  Ps3.attach(notify);
  // Define On Connection Function
  Ps3.attachOnConnect(onConnect);
  // Emulate console as specific MAC address (change as required)
  Ps3.begin("CC:50:E3:96:C4:58");

  // Attach Servo to pin
  srvmtr.attach(SERVO_PIN);

  // Home at 90 degrees
  srvmtr.write(servoPos);
  ledcSetup(motorAChannel, motorFreq, motorResolution);
  ledcAttachPin(enA, motorAChannel);
  // Print to Serial Monitor
  Serial.println("Ready.");
}

void loop() {
  if (!Ps3.isConnected())
    return;
  delay(2000);
}
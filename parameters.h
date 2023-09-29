#ifndef PARAMETERS_H
#define PARAMERERS_H
#include "Arduino.h"
#include "Servo.h"
#include <SoftwareSerial.h>

// Structure for all pins.
// RX and TX pins are the built arduino pins whose calling is not required 
struct parameters {
    const int servoPin1 = 5;
    const int servoPin2 = 6;
    const int switchPin1 = 2;
    const int switchPin2 = 3;
    const int moisturePin = A0;
    const int trigPin = 9;
    const int echoPin = 8;
    int buzzPin = 12;
    String tm;
    String BT_input;
};

parameters param;
int moist_level = 0;

Servo servo1;
Servo servo2;

// Initialization of Modules
void servosetup();
void switchsetup();
void buzz();
void switchInterrupt2();
void ultrasound();
void waterlevelcheck();
void rainwatercheck();
void bluetoothconnection();
void servoffbybluetooth();

void switchsetup() {
  pinMode(param.switchPin1, INPUT_PULLUP);
  pinMode(param.switchPin2, INPUT_PULLUP);

  // Interrupt for Kill Switch
  attachInterrupt(digitalPinToInterrupt(param.switchPin2), switchInterrupt2, FALLING);
}

void servosetup() {
    servo1.attach(param.servoPin1);
    servo2.attach(param.servoPin2);
}
void ultrasoundsetup() {
    pinMode(param.trigPin, OUTPUT);
    pinMode(param.echoPin, INPUT); 
}

// Assume tank length is 2 metres
int tank_length = 2000;
long duration = 20;
int distance;
int eff_distance = tank_length - distance;
void ultrasound(){
  // Ultrasonic Sensor Part
  digitalWrite(param.trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(param.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(param.trigPin, LOW);
  duration = pulseIn(param.echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(eff_distance);

  // eff_distance is the water level measured

}
//and turn the switches according to the water level
void waterlevelcheck(){
  if(eff_distance < 10) {
      // ON the buzzer for 1 second when water is low 
    servo1.write(90);
    digitalWrite(param.buzzPin,HIGH);
    delay(1000);
    digitalWrite(param.buzzPin,LOW);
  }
  // If water is more witch is triggered bore motor is turned OFF with servo 
  if(eff_distance > 1900) {
       // OFF the buzzer for 5 second when water is low 
    servo1.write(0);
    digitalWrite(param.buzzPin,HIGH);
    delay(5000);
    digitalWrite(param.buzzPin,LOW);
  }
}
void rainwatercheck(){
  moist_level = analogRead(param.moisturePin);
  // Reads analog value from 0 to 1024 calibrated to ADC 0 to 877
  // RAIN Condition triggers servo 2 inturn turning the switch ON for rainwater harvesting
  if(moist_level > 250) {
    Serial.print("Servo on switch to ON Rain Harvest Mode");
    // This turns on the top lid to collect rain water
    switchInterrupt2();
      
  }
}

// Second Switch to ON Rain Harvest Mode
void switchInterrupt2() {
  servo2.write(90);
}

#endif

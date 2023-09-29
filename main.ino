#include "parameters.h"
void setup() {
  // Pin setup for servo pins
  servosetup();

  // Pin setup for Interrupt attached Switches
  switchsetup();
  
  // Pin setup for Ultrasonic Sensor
  ultrasoundsetup();

  // Pin setup for Moisture Sensor
  pinMode(param.moisturePin,INPUT);
  Serial.begin(9600);
  
}
void loop() {
  // Measure the water level in the tank
  ultrasound();

  // Conditions of the water level supply -> Reflected on servo motors
  waterlevelcheck();

  // Checking for presence of Rain through moisture sensor
  rainwatercheck();

  // Checking for availabilty of Bluetooth connection
  if (Serial.available())
    {   
        param.BT_input = Serial.readString();   // read input string from bluetooth 
        
        if(eff_distance<10)
        {
          Serial.print("Critical water level");
        }
        
        if(eff_distance>1900)
        {
          Serial.print("Excess water level");
        }
        // "A" input from android APP returns the water level
        if(param.BT_input=="A")
        {
          Serial.print("Current water level ");
          Serial.print(eff_distance);
        }
        // "B" input from android APP stops the servo motor and sets the default BT input to A
        if(param.BT_input=="B")
        {
          Serial.print("You have choosen to stop the motor)");
          servo1.write(0);
          param.BT_input = "A";
        }
      }
  // Motor is set to OFF during Bluetooth Input A
  if(param.BT_input=="A")
  {
    servo1.write(0);
  }
  delay(500);
}


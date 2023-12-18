/*
  ESP 32
  Distance from sensor to mower blades is 50mm.
  Max Sensor Height: 140mm.
  Min Sensor Height: mm.
  
  Teensy 4.1
  Teensy address: 0x08
  Teensy SDA PIN: 40
  Teensy SCL PIN: 41

  FC BOARD
  FC Address: N/A
  FC SDA PIN: IO33 / 9
  FC SCL PIN: IO25 / 10

*/

#include <Arduino.h>
#include <Wire.h>
#include<cmath>
constexpr uint8_t RELAY_1 = GPIO_NUM_14; //Up
constexpr uint8_t RELAY_2 = GPIO_NUM_27; //Down
constexpr unsigned char sensorAddress = 80;
constexpr  float marginOfError = 0.05;
constexpr unsigned char bladeHeightDifference = 50;
constexpr unsigned char packetLength = 2;
constexpr int default_SDA = GPIO_NUM_33;
constexpr int default_SCL = GPIO_NUM_25;

class deckControls {

public:



void moveDeckToInches(float desired_height_in ) { // User can override if deck height sensor is on new port, assumes default port 8.
  
    float current_height_mm = checkHeight() - bladeHeightDifference; // Need a conidtion for what to do if 4 bytes are not avaliable. Using a loop would be blocking? 
    float desired_height_mm = desired_height_in * 25.4; //Converts from inches to mm.
    
    while(current_height_mm > (desired_height_mm + marginOfError)) {
      //Down
      Serial.println("Down Condition called");
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_1, LOW);

      current_height_mm = checkHeight() - bladeHeightDifference;;
    }
    while(current_height_mm < (desired_height_mm - marginOfError)) {
      //Up
      Serial.println("Up condition called");
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, LOW);

       current_height_mm = checkHeight() - bladeHeightDifference;;
    }
    while(current_height_mm == desired_height_mm) {
      Serial.println("Correct height reached");
      return;
    }
   

  
  
  

  }

  float checkHeight(){
     Wire.requestFrom(sensorAddress,  packetLength);
     while (Wire.available() < packetLength);
     byte highByte = Wire.read();
     byte lowByte = Wire.read();
     unsigned short int recievedValue = (highByte << 8) | lowByte;
     float height = recievedValue;
     return height;
    }

};

deckControls myDeck;


void setup () {
   Serial.begin(9600);
   Wire.begin(default_SDA, default_SCL);
   Wire.setClock(100000);
   pinMode(RELAY_1, OUTPUT);
   pinMode(RELAY_2, OUTPUT);
}

void loop () {
   myDeck.moveDeckToInches(2.5);
   //Serial.println(myDeck.checkHeight());

    
  }


/*
  ESP 32
  Distance from sensor to mower blades is 50mm.
  Blade Max Height: 90mm.
  Max Sensor Height: 150mm.
  Min Sensor Height: mm.
  
  Teensy 4.1
  Teensy address: 0x08
  Teensy SDA PIN: 40
  Teensy SCL PIN: 41

  FC BOARD
  FC Address: N/A
  FC SDA PIN: GPIO33 
  FC SCL PIN: GPIO25  

*/

#include <Arduino.h>
#include <Wire.h>
#include<cmath>
constexpr uint8_t RELAY_1 = GPIO_NUM_14; //Up
constexpr uint8_t RELAY_2 = GPIO_NUM_27; //Down
constexpr unsigned char sensorAddress = 80;
constexpr  float tolerance = 3.0; // mm
constexpr unsigned char bladeHeightDifference = 60;
constexpr unsigned char packetLength = 2;
constexpr unsigned char calibrationFactor = 3;
constexpr int default_SDA = GPIO_NUM_33;
constexpr int default_SCL = GPIO_NUM_25;

class deckControls {

public:



void moveDeckUp() {
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, LOW);
}

void moveDeckDown(){
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_1, LOW);
}

void stopDeck(){
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
}

void moveDeckToInches(float desired_height_in ) { 
  
     // Need a conidtion for what to do if 4 bytes are not avaliable. Using a loop would be blocking? 
    float desired_height_mm = desired_height_in * 25.4; //Converts from inches to mm.
    bool flag = true;
    while(flag == true) {
      
      float current_height_mm = checkHeight() - bladeHeightDifference - calibrationFactor;
      int error =  current_height_mm - desired_height_mm;

      if(error > tolerance) {
        moveDeckDown();
        Serial.println("Moving Down");

      }
      else if(error < tolerance) {
        moveDeckUp();
        Serial.println("Moving up");
      }
      else{
        stopDeck();
        Serial.println("Deck stopping");
        flag = false;
      }
      
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
   Serial.println("Main Loop Running");
   delay(500);
  }


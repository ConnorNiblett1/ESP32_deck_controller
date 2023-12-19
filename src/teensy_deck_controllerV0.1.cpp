/*
 
  Teensy 4.1
  Address: 0x08
  TODO: Blink Led when recieving data. Stick something at the end of the read function to verify it's actually working. 
*/


#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>

constexpr uint16_t ULTRASONIC_SENSOR_BAUD_RATE = 9600;
constexpr unsigned char sensorAddress = 80;
int ledState = LOW;



void setupSensor(HardwareSerial &serial) {
  serial.begin(ULTRASONIC_SENSOR_BAUD_RATE);
  
}

void blinkBuiltIn(unsigned short int ledPin ){
  pinMode(ledPin, OUTPUT);
  
  unsigned long previousMillis =0;
  constexpr long interval = 10000;
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(ledState == LOW){
      ledState = HIGH;
    }
      else{
        ledState = LOW;
      }
  }
  digitalWrite(ledPin, ledState);
}

unsigned short int readSensorValue(HardwareSerial &serial) {
  // float result = -1;
  int data[4] = {};
 
  while (true) { 
    while (serial.available() > 12) {
    data[0] = serial.read();

    if (data[0] != 0xff) {
      continue;
    }

    data[1] = serial.read();
    data[2] = serial.read();
    data[3] = serial.read();

    // auto sum = (data[0] + data[1] + data[2]) & 0x00ff;

    // TODO: verify checksum

    unsigned short int distance_mm = (data[1] << 8) + data[2];
     blinkBuiltIn(LED_BUILTIN);

    return distance_mm;
   }
  }
}

unsigned short int mostRecentRead;

void requestEvent(){
  byte highByte = highByte(mostRecentRead);
  byte lowByte = lowByte(mostRecentRead);
  Wire.write(highByte);
  Wire.write(lowByte);
}


void setup(){
    Serial.begin(9600);
    Wire.begin(sensorAddress);
    Wire.setClock(50000);
    setupSensor(Serial8);
    Wire.onRequest(requestEvent);
}



void loop(){
  mostRecentRead = readSensorValue(Serial8);
  
}


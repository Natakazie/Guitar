#include <Arduino.h>
#include <FastLED.h>
#include "GY521.h"

// Instantiates GY521 sensor object with address 0x68
GY521 sensor(0x68);

uint32_t counter = 0;

// Define the pins for the Bluetooth module
#define RXBluetooth  DD3
#define TXBluetooth  DD2
#define ClockMPU  SCL
#define DataMPU  SDA

//definitions MPU
#define  DirRight  1
#define  DirLeft  2
#define DirUp  3
#define DirDown 4
#define bar 0.75 //sensitivity threshold

//definions KY-024 hall sensor
#define HallDigital 3
#define HallAnalog A0
#define HallDigital2 4
#define HallAnalog2 A1

//definitions LED
#define NUM_LEDS 5
#define DATA_PIN 12
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];


int buttonPressed;
float correction;

void initKY024(){ //init hall sensor
  pinMode(HallAnalog, INPUT);
  pinMode(HallDigital, INPUT); 
  pinMode(HallAnalog2, INPUT);
  pinMode(HallDigital2, INPUT);
}

void initMPU(){
  sensor.reset();
  sensor.begin();
  delay(1);
  sensor.read();
  // Stores the negative of the X acceleration as correction
  correction = -sensor.getAccelX();
}

void initLED(){
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
}

void handleMPU(){
    // Checks the direction in which the MPU is tilted and updates the `buttonPressed` variable accordingly
    if(sensor.getAccelX() + correction > bar){
    buttonPressed = DirDown;
  }else   if(sensor.getAccelX() +correction < -bar){
    buttonPressed = DirUp;
  }else   if(sensor.getAccelY() > bar){
    buttonPressed = DirLeft;
  }else   if(sensor.getAccelY() < -bar){
    buttonPressed = DirRight;
  }else if (sensor.getAccelZ() > bar || sensor.getAccelZ() < -bar){
    buttonPressed= 0;
  }
}
  void sendData(){
    String toSend = "";
    Serial.write(buttonPressed);
    if(digitalRead(HallDigital) > 0){
        toSend+="D;";
    }else{
      toSend+="d;";
    }
    if(digitalRead(HallDigital2) > 0){
        toSend+="A;";
    }else{
      toSend+="a;";
    }
  }
  /*
  Serial.print( correction);
  Serial.print(" ");
  Serial.print( sensor.getAccelX());
  Serial.print(" ");
  Serial.print( sensor.getAccelY());
  Serial.print(" ");
  Serial.println(buttonPressed);
*/


void handleKY024(){ //hall sensor function
  // Read the analog and digital values from the Hall sensor
  int analog = analogRead(HallAnalog); 
  int digital = digitalRead(HallDigital);
  int analog2 = analogRead(HallAnalog2); 
  int digital2 = digitalRead(HallDigital2);  
  leds[0] = (digital == 1) ? CRGB::Green : CRGB::Black;
  //update LED
  FastLED.show();
}


void setup() {
  Serial.begin(9600);    // Starts the serial communication with a baud rate of 9600
  Serial.println("Initializing Connection");
  initMPU();
  initKY024();
  initLED();
}

void loop() {
  //sensor.reset();
  sensor.read();
  handleKY024();
  handleMPU();
  sendData();
  unsigned long tar = millis()+20;
  while(millis() < tar){

  }
}

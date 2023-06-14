#include <Arduino.h>
#include <FastLED.h>
#include <SoftwareSerial.h>
#include "GY521.h"
#include "Adafruit_DRV2605.h"

// Instantiates GY521 sensor object with address 0x68
GY521 sensor(0x68);

Adafruit_DRV2605 drv;

uint32_t counter = 0;

// Define the pins for the Bluetooth module
#define RXBluetooth  DD3
#define TXBluetooth  DD2

SoftwareSerial blueSerial(RXBluetooth, TXBluetooth); // RX, TX

#define ClockMPU  SCL
#define DataMPU  SDA

//definitions MPU
#define  DirRight  1
#define  DirLeft  2
#define DirUp  3
#define DirDown 4
#define bar 0.75 //sensitivity threshold

//definions KY-024 hall sensor
#define HallDigital0 3
#define HallAnalog0 A0
#define HallDigital1 4
#define HallAnalog1 A1
#define HallDigital2 5
#define HallAnalog2 A2
#define HallDigital3 6
#define HallAnalog3 A3
#define HallDigital4 7
#define HallAnalog4 A4

//definitions LED
#define NUM_LEDS 5
#define DATA_PIN 12
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];




void initKY024(){ //init hall sensor
  pinMode(HallAnalog0, INPUT);
  pinMode(HallDigital0, INPUT); 
  pinMode(HallAnalog1, INPUT);
  pinMode(HallDigital1, INPUT);
  pinMode(HallAnalog2, INPUT);
  pinMode(HallDigital2, INPUT); 
  pinMode(HallAnalog3, INPUT);
  pinMode(HallDigital3, INPUT);
  pinMode(HallAnalog4, INPUT);
  pinMode(HallDigital4, INPUT);
}
float correctionX;
float correctionY;
float correctionZ;
void initMPU(){
  sensor.reset();
  sensor.begin();
  //first read reads nothing but zeros;
  sensor.read();
  unsigned long time = millis()+20;
  while(millis() < time){

  }
  sensor.read();
  correctionX = -sensor.getAccelX();
  correctionY = -sensor.getAccelY();
  correctionZ = -sensor.getAccelZ();
  Serial.print(correctionX);Serial.print(" "); Serial.print(correctionY);Serial.print(" "); Serial.println(correctionZ);
  // Stores the negative of the X acceleration as correctionX
}

void initLED(){
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
}
void initDRV(){
  Serial.println("Adafruit DRV2605 Basic test");
  if (! drv.begin()) {
    Serial.println("Could not find DRV2605");
    while (1) delay(10);
  }
 
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 
  drv.setWaveform(0, 56);  // play effect  
    
}
bool neutral = false;
int xAxis = 0;
int yAxis = 0;
void handleMPU(){
    // Checks the direction in which the MPU is tilted and updates the `buttonPressed` variable accordingly
  sensor.read();
   neutral = false;
   if (sensor.getAccelZ()+correctionZ > bar || sensor.getAccelZ()+correctionZ < -bar){
    neutral = true;
    xAxis = 0;
    yAxis = 0;
    drv.go();
   }
    if(sensor.getAccelX() + correctionX > bar && xAxis!= 1){
    xAxis = 1;
    drv.go();
  }else   if(sensor.getAccelX() +correctionX < -bar && xAxis !=-1){
    xAxis = -1;
    drv.go();
  }
  if(sensor.getAccelY()+correctionY > bar && yAxis != -1){
    yAxis= -1;
    drv.go();
  }else   if(sensor.getAccelY() +correctionY < -bar && yAxis != 1){
    yAxis = 1;
    drv.go();
  }
  Serial.print(xAxis);Serial.print(" ");
  Serial.print( yAxis);Serial.print( " ");Serial.println(neutral);
}

  void sendData(){
    String toSend = "";
    if(digitalRead(HallDigital0) > 0){
        toSend+="D;";
        leds[0] = CRGB::Green; 
    }else{
      toSend+="d;";
      leds[0] = CRGB::Green;
    }
    if(digitalRead(HallDigital1) > 0){
        toSend+="A;";
        leds[1] = CRGB::Red;
    }else{
      toSend+="a;";
      leds[1] = CRGB::Black;
    }
    if(digitalRead(HallDigital2) > 0){
        toSend+="S;";
        leds[2] = CRGB::Yellow; 
    }else{
      toSend+="s;";
      leds[2] = CRGB::Black;
    }
    if(digitalRead(HallDigital3) > 0){
        toSend+="F;";
        leds[3] = CRGB::Blue;
    }else{
      toSend+="f;";
      leds[3] =CRGB::Black;
    }if(digitalRead(HallDigital4) > 0){
        toSend+="G;";
        leds[4] = CRGB::Orange;
    }else{
      toSend+="g;";
      leds[4] = CRGB::Black;
    }
    FastLED.show();
    if(neutral){
      toSend+="neutral;";
    }
    if(xAxis == 1){
      toSend+="up;";
    }else if(xAxis == -1){
      toSend+="down;";
    }
    if(yAxis == 1){
      toSend+="left;";
    }else if(yAxis == -1){
      toSend+="right;";
    }
    Serial.println(toSend);
  }
  /*
  Serial.print( correctionX);
  Serial.print(" ");
  Serial.print( sensor.getAccelX());
  Serial.print(" ");
  Serial.print( sensor.getAccelY());
  Serial.print(" ");
  Serial.println(buttonPressed);
*/


// void handleKY024(){ //hall sensor function
//   // Read the analog and digital values from the Hall sensor
//   int analog = analogRead(HallAnalog); 
//   int digital = digitalRead(HallDigital);
//   int analog2 = analogRead(HallAnalog2); 
//   int digital2 = digitalRead(HallDigital2);  
  
//   //update LED
//   FastLED.show();
// }

void setup() {
  Serial.begin(9600);    // Starts the serial communication with a baud rate of 9600
  blueSerial.begin(9600);
  Serial.println("Initializing Connection");
  blueSerial.println("Initializing Bluetooth Connection");
  blueSerial.write("AT+NAMEguitargloves");
  blueSerial.write("AT+PIN2023");
  initMPU();
  //initKY024();
  initLED();
  initDRV();
  Serial.println("Initialization Complete");
}

void loop() {
  //sensor.reset();
  //handleKY024();
  handleMPU();
  sendData();
    
  unsigned long time = millis()+1000;
  while(millis() < time){

  }
  // unsigned long tar = millis()+20;
  // while(millis() < tar){
  // }
}

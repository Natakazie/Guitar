#include <Arduino.h>
#include "GY521.h"

GY521 sensor(0x68);

uint32_t counter = 0;

float ax, max, ay, az;
float gx, gy, gz;
float t;



// put function declarations here:
#define RXBluetooth  DD3
#define TXBluetooth  DD2
#define ClockMPU  SCL
#define DataMPU  SDA

//definitions MPU
#define  DirRight  1
#define  DirLeft  2
#define DirUp  3
#define DirDown 4
#define bar 0.75

//definions KY-024
#define HallDigital 3
#define HallAnalog A0


int buttonPressed;
int myFunction(int, int);
float correction;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initializing Connection");
  //Initiate MPU
  sensor.reset();
  sensor.begin();
  delay(1);
  sensor.read();
  correction = -sensor.getAccelX();
  //Initiate KY-024 Hallsensor 
  pinMode(HallAnalog, INPUT);
  pinMode(HallDigital, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //sensor.reset();
  int analog = analogRead (HallAnalog); 
  int digital = digitalRead (HallDigital);
    
  //... und an dieser Stelle ausgegeben
  Serial.print ("Analoger Spannungswert:"); Serial.print (analog);  Serial.print ("V, ");
  Serial.print ("Grenzwert:");Serial.println(digital);
  sensor.read();

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
  /*
  Serial.print( correction);
  Serial.print(" ");
  Serial.print( sensor.getAccelX());
  Serial.print(" ");
  Serial.print( sensor.getAccelY());
  Serial.print(" ");
  Serial.println(buttonPressed);
*/
  /*
  if(tempX > ax){
  ax = tempX;
  }
  if (tempX < max){
    max = tempX;
  }
  //ay = sensor.getAccelY();  az = sensor.getAccelZ();
  Serial.print(ax);
  Serial.print(" ");
  Serial.print(max); //Serial.print(" "); Serial.print(ay); Serial.print(" "); Serial.print(az);
  Serial.println();
  */
  delay(200);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

/*
#include "GY521.h"

GY521 sensor(0x68);

uint32_t counter = 0;

float ax, ay, az;
float gx, gy, gz;
float t;


void setup()
{
  Serial.begin(9600
);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("GY521_LIB_VERSION: ");
  Serial.println(GY521_LIB_VERSION);

  Wire.begin();
  delay(100);
  if (sensor.wakeup() == false)
  {
    Serial.println("Could not connect to GY521");
  }
  //  adjust when needed.
  sensor.setAccelSensitivity(0);  //  2g
  sensor.setGyroSensitivity(0);   //  250 degrees/s
  sensor.setThrottle(false);

  //  set all calibration errors to zero
  sensor.axe = 0;
  sensor.aye = 0;
  sensor.aze = 0;
  sensor.gxe = 0;
  sensor.gye = 0;
  sensor.gze = 0;

  Serial.println("\n\nReading calibration numbers...");
}


void loop()
{
  ax = ay = az = 0;
  gx = gy = gz = 0;
  t = 0;
  for (int i = 0; i < 100; i++)
  {
    sensor.read();
    ax -= sensor.getAccelX();
    ay -= sensor.getAccelY();
    az -= sensor.getAccelZ();
    gx -= sensor.getGyroX();
    gy -= sensor.getGyroY();
    gz -= sensor.getGyroZ();
    t += sensor.getTemperature();
  }

  if (counter % 10 == 0)
  {
    Serial.println("\n\tCOPY CODE SNIPPET");
    Serial.print("sensor.axe = ");
    Serial.print(sensor.axe, 7);
    Serial.print(";\n");

    Serial.print("sensor.aye = ");
    Serial.print(sensor.aye, 7);
    Serial.print(";\n");

    Serial.print("sensor.aze = ");
    Serial.print(sensor.aze, 7);
    Serial.print(";\n");
    
    Serial.print("sensor.gxe = ");
    Serial.print(sensor.gxe, 7);
    Serial.print(";\n");

    Serial.print("sensor.gye = ");
    Serial.print(sensor.gye, 7);
    Serial.print(";\n");

    Serial.print("sensor.aze = ");
    Serial.print(sensor.gze, 7);
    Serial.print(";\n");

    Serial.println("\taxe\taye\taze\tgxe\tgye\tgze\tT");
  }

  if (counter % 10 == 0)
  {
  Serial.print(counter);
  Serial.print('\t');
  Serial.print(ax * 0.01, 3);
  Serial.print('\t');
  Serial.print(ay * 0.01, 3);
  Serial.print('\t');
  Serial.print(az * 0.01, 3);
  Serial.print('\t');
  Serial.print(gx * 0.01, 3);
  Serial.print('\t');
  Serial.print(gy * 0.01, 3);
  Serial.print('\t');
  Serial.print(gz * 0.01, 3);
  Serial.print('\t');
  Serial.print(t * 0.01, 2);
  Serial.println();
  }
  //  adjust calibration errors so table should get all zero's.
  sensor.axe += ax * 0.01;
  sensor.aye += ay * 0.01;
  sensor.aze += az * 0.01;
  sensor.gxe += gx * 0.01;
  sensor.gye += gy * 0.01;
  sensor.gze += gz * 0.01;

  counter++;

}

*/
/*
#include "Wire.h" // This library allows you to communicate with I2C devices.

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data

char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
  Serial.println();
  
  // delay
  delay(100);}
  
*/
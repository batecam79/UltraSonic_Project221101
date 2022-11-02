#include <SoftwareSerial.h>
//#include <NeoSWSerial.h>

//relay_Pin setting --------------------------------------------

const int relay_1 = 6;
const int relay_2 = 7;
const int relay_3 = 8;

//sensor_1 setting --------------------------------------------

unsigned int S1Head = 0;
unsigned int S1MSByteDist = 0;
unsigned int S1LSByteDist = 0;
unsigned int S1checksum = 0;
unsigned int S1_Dist = 0;


//sensor_2 setting --------------------------------------------
const byte tx2Pin = 2;
const byte rx2Pin = 3;

SoftwareSerial sensor_2 (rx2Pin, tx2Pin);

unsigned int S2Head = 0;
unsigned int S2MSByteDist = 0;
unsigned int S2LSByteDist = 0;
unsigned int S2checksum = 0;
unsigned int S2_Dist = 0;


//sensor_3 setting --------------------------------------------
const byte tx3Pin = 4;
const byte rx3Pin = 5;

SoftwareSerial sensor_3 (rx3Pin, tx3Pin);

unsigned int S3Head = 0;
unsigned int S3MSByteDist = 0;
unsigned int S3LSByteDist = 0;
unsigned int S3checksum = 0;
unsigned int S3_Dist = 0;

//-----------------timer-------------------------------

unsigned long S1_pS = 0;    //sensor1 previousMillis
unsigned long S1_cS = 0;    //sensor1 currentMillis

unsigned long S2_pS = 0;    //sensor1 previousMillis
unsigned long S2_cS = 0;    //sensor1 currentMillis

unsigned long S3_pS = 0;    //sensor1 previousMillis
unsigned long S3_cS = 0;    //sensor1 currentMillis

const long Relay1_Stopinterval = 10000;      //此时间为上次继电器针脚LOW之后的间隔时间，再这个间隔时间后才会继续响应digitalWrite
const long Relay2_Stopinterval = 10000;
const long Relay3_Stopinterval = 10000;

const long Relay1_Oninterval = 200;
const long Relay2_Oninterval = 200;
const long Relay3_Oninterval = 200;

int relay1_FirstOn = 0;
int relay2_FirstOn = 0;
int relay3_FirstOn = 0;

bool relay1State = LOW;
bool relay2State = LOW;
bool relay3State = LOW;


//---------------------------setup---------------------------------
void setup()  {

    // Define pin modes for TX and RX
    //0,1 hardwareSerial
    pinMode(rx2Pin, INPUT);
    pinMode(tx2Pin, OUTPUT);
    pinMode(rx3Pin, INPUT);
    pinMode(tx3Pin, OUTPUT);
    pinMode(relay_1, OUTPUT);
    pinMode(relay_2, OUTPUT);
    pinMode(relay_3, OUTPUT);

    // Set the baud rate for the SoftwareSerial object
    Serial.begin(9600);
    sensor_2.begin(9600);
    sensor_3.begin(9600);

    digitalWrite(relay_1, relay1State);
    digitalWrite(relay_2, relay2State);
    digitalWrite(relay_3, relay3State);


}

void loop() {

  Serial.write(0x55);
  delay(60);
  while(Serial.available() >= 4) //// Check reception of 4 bytes
  { 
    S1Head = Serial.read();
    S1MSByteDist = Serial.read(); // 
    S1LSByteDist  = Serial.read();
    S1checksum  = Serial.read();
    S1_Dist  = S1MSByteDist * 256 + S1LSByteDist; // Distance
    // Serial.print("Distance-1: ");
    // Serial.print(S1_Dist, DEC);
    // Serial.print(" mm    ");
    // Serial.println();
    if((S1_Dist > 1000) && (S1_Dist < 2000) && (relay1State == LOW)) // Check if the distance is in expected range
    {   
      if(relay1_FirstOn == 0){      //turn on relay1 for 10sec and set relay1_FirstON = 1
        digitalWrite(relay_1, HIGH);
        relay1_FirstOn = 1;
        relay1State = HIGH;
        S1_pS = millis();
      }
    }

  }



  sensor_2.write(0x55);
  sensor_2.listen();
  delay(60);
  while(sensor_2.available() >= 4) // Check reception of 4 bytes
  {
    S2Head = sensor_2.read();
    S2MSByteDist = sensor_2.read(); // 
    S2LSByteDist  = sensor_2.read();
    S2checksum  = sensor_2.read();
    S2_Dist  = S2MSByteDist * 256 + S2LSByteDist; // Distance
    // Serial.print("Distance-2: ");
    // Serial.print(S2_Dist, DEC);
    // Serial.print(" mm    ");
    // Serial.println();
    
    if((S2_Dist > 1000) && (S2_Dist < 2000)) // Check if the distance is in expected range
    {
      digitalWrite(relay_2, HIGH);
    }
    else{
      digitalWrite(relay_2, LOW);
    }
  }  
  
  sensor_3.write(0x55);
  sensor_3.listen();
  delay(60);
  while(sensor_3.available() >= 4) // Check reception of 4 bytes
  {
    S3Head = sensor_3.read();
    S3MSByteDist = sensor_3.read(); // 
    S3LSByteDist  = sensor_3.read();
    S3checksum  = sensor_3.read();
    S3_Dist  = S3MSByteDist * 256 + S3LSByteDist; // Distance
    // Serial.print("Distance-3: ");
    // Serial.print(S3_Dist, DEC);
    // Serial.print(" mm    ");
    // Serial.println();
    if((S3_Dist > 1000) && (S3_Dist < 2000)) // Check if the distance is in expected range
    {
      digitalWrite(relay_3, HIGH);
    }
    else{
      digitalWrite(relay_3, LOW);
      }
  }

}

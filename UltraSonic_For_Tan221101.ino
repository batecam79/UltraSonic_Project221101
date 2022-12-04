#include <SoftwareSerial.h>
//#include <millisDelay.h>
#include <MsTimer2.h>     //timer lib

const int relay_1 = 6;    //relay 1 Pin
const int relay_2 = 7;    //relay 2 Pin
const int relay_3 = 8;    //relay 3 Pin

//sensor_1 setting --------------------------------------------
const byte tx1Pin = 10;    
const byte rx1Pin = 11;

SoftwareSerial sensor_1 (rx1Pin, tx1Pin);

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


int Current_Serial_Job = 1;	
	//0:Waiting
	//1:Send Data From Serail 1
	//2:Read Data From Serail 1
	//3:Send Data From Serail 2
	//4:Read Data From Serail 2
	//5:Send Data From Serail 3
	//6:Read Data From Serail 3

int Current_Relay1_Job = 0;
	//0:Do Nothing
	//1:Close Relay
	//2:Delay 10 Second
int Current_Relay1_Count = 0;

int Current_Relay2_Job = 0;
	//0:Do Nothing
	//1:Close Relay
	//2:Delay 10 Second
int Current_Relay2_Count = 0;

int Current_Relay3_Job = 0;
	//0:Do Nothing
	//1:Close Relay
	//2:Delay 10 Second
int Current_Relay3_Count = 0;

//中断服务程序
void onTimer()
{
	switch (Current_Serial_Job){
		case 1:
			sensor_1.write(0x55);    //sensor trigger byte
      sensor_1.listen();       //open softserial and wait for incoming data
			break;

		case 2:
			if (sensor_1.available() >= 4){	//Check Serail 1
				S1Head = sensor_1.read();
        S1MSByteDist = sensor_1.read(); // 
        S1LSByteDist  = sensor_1.read();
        S1checksum  = sensor_1.read();
        S1_Dist  = S1MSByteDist * 256 + S1LSByteDist; // read Distance
        Serial.print("Distance-1: ");
        Serial.print(S1_Dist, DEC);
        Serial.print(" mm    ");
        Serial.println();
				//Read Serial 1 Data and Calculate Distance
				if ((S1_Dist>1000) && (S1_Dist<2000)){	//Check if the distance is in expected range
					Current_Relay1_Job=1;
					Current_Relay1_Count=0;
			        digitalWrite(relay_1, HIGH);	//Open Relay
				}
			}
			break;

		case 3:
			sensor_2.write(0x55);
      sensor_2.listen();
			break;
		case 4:

			if (sensor_2.available() >= 4){	//Check Serail 2
				S2Head = sensor_2.read();
        S2MSByteDist = sensor_2.read(); // 
        S2LSByteDist  = sensor_2.read();
        S2checksum  = sensor_2.read();
        S2_Dist  = S2MSByteDist * 256 + S2LSByteDist; // Distance
        Serial.print("Distance-2: ");
        Serial.print(S2_Dist, DEC);
        Serial.print(" mm    ");
        Serial.println();
				//Read Serial 2 Data and Calculate Distance
				if ((S2_Dist>1000) && (S2_Dist<2000)){	//Check if the distance is in expected range
					Current_Relay2_Job=1;
					Current_Relay2_Count=0;
			        digitalWrite(relay_2, HIGH);	//Open Relay
				}
			}
			break;
		case 5:
			sensor_3.write(0x55);
      sensor_3.listen();
			break;
		case 6:

			if (sensor_3.available() >= 4){	//Check Serail 3
				S3Head = sensor_3.read();
        S3MSByteDist = sensor_3.read(); // 
        S3LSByteDist = sensor_3.read();
        S3checksum = sensor_3.read();
        S3_Dist  = S3MSByteDist * 256 + S3LSByteDist; // Distance
        Serial.print("Distance-3: ");
        Serial.print(S3_Dist, DEC);
        Serial.print(" mm    ");
        Serial.println();
				//Read Serial 3 Data and Calculate Distance
				if ((S3_Dist>1000) && (S3_Dist<2000)){	//Check if the distance is in expected range
					Current_Relay3_Job=1;
					Current_Relay3_Count=0;
			        digitalWrite(relay_3, HIGH);	//Open Relay
				}
			}
			break;
	}

	if ((Current_Relay1_Job > 0) && (Current_Relay2_Job > 0) && (Current_Relay3_Job > 0)){		//Three Relay all is Opened Or Waiting
		Current_Serial_Job = 0;		//Waiting one Serial can Send Data
  }
  else{
    if (++Current_Serial_Job > 6)				//Next Step>6 go to Serial1 Send Data
      Current_Serial_Job=1;
    if ((Current_Serial_Job == 1) && (Current_Relay1_Job>0))		//Next Step is Serial 2 Send Data ,but Relay 2 is Opened Or Waiting
      Current_Serial_Job=3;
    if ((Current_Serial_Job == 3) && (Current_Relay2_Job>0))		//Next Step is Serial 2 Send Data ,but Relay 2 is Opened Or Waiting
      Current_Serial_Job=5;
    if ((Current_Serial_Job == 5) && (Current_Relay3_Job>0))		//Next Step is Serial 3 Send Data ,but Relay 3 is Opened Or Waiting
      Current_Serial_Job=1;
  }

	if (Current_Relay1_Job > 0){
		Current_Relay1_Count++;
		if ((Current_Relay1_Job == 1) && (Current_Relay1_Count >= 20)){		//relay open for 1200ms(60msx20=1200ms)
	        digitalWrite(relay_1, LOW);
			Current_Relay1_Job = 2;
			Current_Relay1_Count = 0;
		}
		else if ((Current_Relay1_Job == 2) && (Current_Relay1_Count >= 200)){		//sensor stop for 12s(60msx200=12000ms)
			Current_Relay1_Job = 0;
			Current_Relay1_Count = 0;
		}
	}

	if (Current_Relay2_Job > 0){
		Current_Relay2_Count++;
		if ((Current_Relay2_Job == 1) && (Current_Relay2_Count >= 20)){		//relay open for 1200ms(60msx20=1200ms)
	        digitalWrite(relay_2, LOW);
			Current_Relay2_Job = 2;
			Current_Relay2_Count = 0;
		}
		else if ((Current_Relay2_Job == 2) && (Current_Relay2_Count >= 200)){		//sensor stop for 12s(60msx200=12000ms)
			Current_Relay2_Job = 0;
			Current_Relay2_Count = 0;
		}
	}

	if (Current_Relay3_Job > 0){
		Current_Relay3_Count++;
		if ((Current_Relay3_Job == 1) && (Current_Relay3_Count >= 20)){		//relay open for 1200ms(60msx20=1200ms)
	        digitalWrite(relay_3, LOW);
			Current_Relay3_Job = 2;
			Current_Relay3_Count = 0;
		}
		else if ((Current_Relay3_Job == 2) && (Current_Relay3_Count >= 200)){		//sensor stop for 12s(60msx200=12000ms)
			Current_Relay3_Job = 0;
			Current_Relay3_Count = 0;
		}
	}
}

void setup()
{
  pinMode(rx2Pin, INPUT);
  pinMode(tx2Pin, OUTPUT);
  pinMode(rx3Pin, INPUT);
  pinMode(tx3Pin, OUTPUT);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);

  
  Serial.begin(9600);       //initialize Serial for debug
  sensor_1.begin(9600);     //initialize Serial for sensor1
  sensor_2.begin(9600);     //initialize Serial for sensor2
  sensor_3.begin(9600);     //initialize Serial for sensor3
 
  MsTimer2::set(60, onTimer); //Set Timer=60ms，after 60ms onTimer_Serial1(); The Timer must>=60ms for sensor reading.
  MsTimer2::start(); //Start Timer
}

void loop(){

}

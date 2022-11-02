/*
    Your code description here
    (c) you, 20XX
    All rights reserved.

    Functionality: 
    
    Version log:
    
    20XX-MM-DD:
        v1.0.0  - Initial release
        
*/
// ==== DEFINES ===================================================================================

// ==== Debug and Test options ==================
//#define _DEBUG_
//#define _TEST_

//===== Debugging macros ========================
#ifdef _DEBUG_
#define SerialD Serial
#define _PM(a) SerialD.print(millis()); SerialD.print(": "); SerialD.println(a)
#define _PP(a) SerialD.print(a)
#define _PL(a) SerialD.println(a)
#define _PX(a) SerialD.println(a, HEX)
#else
#define _PM(a)
#define _PP(a)
#define _PL(a)
#define _PX(a)
#endif




// ==== INCLUDES ==================================================================================

// ==== Uncomment desired compile options =================================
// ----------------------------------------
// The following "defines" control library functionality at compile time,
// and should be used in the main sketch depending on the functionality required
// Should be defined BEFORE #include <TaskScheduler.h>  !!!
//
// #define _TASK_TIMECRITICAL       // Enable monitoring scheduling overruns
// #define _TASK_SLEEP_ON_IDLE_RUN  // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass
// #define _TASK_STATUS_REQUEST     // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS            // Compile with support for wdt control points and task ids
// #define _TASK_LTS_POINTER        // Compile with support for local task storage pointer
// #define _TASK_PRIORITY           // Support for layered scheduling priority
// #define _TASK_MICRO_RES          // Support for microsecond resolution
// #define _TASK_STD_FUNCTION       // Support for std::function (ESP8266 ONLY)
// #define _TASK_DEBUG              // Make all methods and variables public for debug purposes
// #define _TASK_INLINE             // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
// #define _TASK_TIMEOUT            // Support for overall task timeout
// #define _TASK_OO_CALLBACKS       // Support for callbacks via inheritance
// #define _TASK_EXPOSE_CHAIN       // Methods to access tasks in the task chain
// #define _TASK_SCHEDULING_OPTIONS // Support for multiple scheduling options
// #define _TASK_DEFINE_MILLIS      // Force forward declaration of millis() and micros() "C" style
// #define _TASK_EXTERNAL_TIME      // Custom millis() and micros() methods
// #define _TASK_THREAD_SAFE        // Enable additional checking for thread safety
// #define _TASK_SELF_DESTRUCT      // Enable tasks to "self-destruct" after disable

#include <TaskScheduler.h>

#include <SoftwareSerial.h>



// ==== GLOBALS ===================================================================================

const int relay_1 = 6;
const int relay_2 = 7;
const int relay_3 = 8;

//sensor_1 setting --------------------------------------------
// const byte tx1Pin = 11;
// const byte rx1Pin = 12;

// SoftwareSerial sensor_1 (tx, tx1Pin);

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


//sensor_2 setting --------------------------------------------
const byte tx3Pin = 4;
const byte rx3Pin = 5;

SoftwareSerial sensor_3 (rx3Pin, tx3Pin);

unsigned int S3Head = 0;
unsigned int S3MSByteDist = 0;
unsigned int S3LSByteDist = 0;
unsigned int S3checksum = 0;
unsigned int S3_Dist = 0;








// ==== Scheduler ==============================
Scheduler ts;

void task1Callback();


void Trig2Callback();
bool Trig2_OnEnble();
void Trig2_OnDisable();

void Trig3Callback();
bool Trig3_OnEnble();
void Trig3_OnDisable();

// ==== Scheduling defines (cheat sheet) =====================
/*
  TASK_MILLISECOND  - one millisecond in millisecond/microseconds
  TASK_SECOND       - one second in millisecond/microseconds
  TASK_MINUTE       - one minute in millisecond/microseconds
  TASK_HOUR         - one hour in millisecond/microseconds
  TASK_IMMEDIATE    - schedule task to runn as soon as possible
  TASK_FOREVER      - run task indefinitely
  TASK_ONCE         - run task once
  TASK_NOTIMEOUT    - set timeout interval to No Timeout
  
  TASK_SCHEDULE     - schedule is a priority, with "catch up" (default)
  TASK_SCHEDULE_NC  - schedule is a priority, without "catch up"
  TASK_INTERVAL     - interval is a priority, without "catch up"
  
  TASK_SR_OK        - status request triggered with an OK code (all good)
  TASK_SR_ERROR     - status request triggered with an ERROR code
  TASK_SR_CANCEL    - status request was cancelled
  TASK_SR_ABORT     - status request was aborted
  TASK_SR_TIMEOUT   - status request timed out
*/

// ==== Task definitions ========================
Task Trig1 ();                                                                                        //硬件串口发送0x55触发US动作
Task Trig2 (TASK_IMMEDIATE, TASK_FOREVER, &Trig2Callback, &ts, true, &Trig2_OnEnble, &Trig2_OnDisable);       //软件串口1发送0x55触发US动作
Task Trig3 (TASK_IMMEDIATE, TASK_FOREVER, &Trig3Callback, &ts, true, &Trig3_OnEnble, &Trig3_OnDisable);       //软件串口2发送0x55触发US动作



bool Trig2_OnEnble() {
	// tLED.setInterval( 200 + random(801) );
	// tLED.setCallback( &LEDOn);
	// tLED.enable();

	// return true; // Task should be enabled
}

void Trig2_OnDisable() {
	// tLED.disable();
}


bool Trig3_OnEnble() {
	// tLED.setInterval( 200 + random(801) );
	// tLED.setCallback( &LEDOn);
	// tLED.enable();

	// return true; // Task should be enabled
}

void Trig3_OnDisable() {
	// tLED.disable();
}



// ==== CODE ======================================================================================

/**************************************************************************/
/*!
    @brief    Standard Arduino SETUP method - initialize sketch
    @param    none
    @returns  none
*/
/**************************************************************************/
void setup() {
  // put your setup code here, to run once:
#if defined(_DEBUG_) || defined(_TEST_)
  Serial.begin(115200);
  delay(2000);
  _PL("Scheduler Template: setup()");
#endif

    Serial.begin(9600);
    // Define pin modes for TX and RX
    // pinMode(rx1Pin, INPUT);
    // pinMode(tx1Pin, OUTPUT);
    pinMode(rx2Pin, INPUT);
    pinMode(tx2Pin, OUTPUT);
    pinMode(rx3Pin, INPUT);
    pinMode(tx3Pin, OUTPUT);
    pinMode(relay_1, OUTPUT);
    pinMode(relay_2, OUTPUT);
    pinMode(relay_3, OUTPUT);


    // Set the baud rate for the SoftwareSerial object
    // sensor_1.begin(9600);
    sensor_2.begin(9600);
    sensor_3.begin(9600);

}


/**************************************************************************/
/*!
    @brief    Standard Arduino LOOP method - using with TaskScheduler there 
              should be nothing here but ts.execute()
    @param    none
    @returns  none
*/
/**************************************************************************/
void loop() {
  ts.execute();
}


/**************************************************************************/
/*!
    @brief    Callback method of task1 - explain
    @param    none
    @returns  none
*/
/**************************************************************************/
void task1Callback() {

//  task code
}


/**************************************************************************/
/*!
    @brief    Callback method of task2 - explain
    @param    none
    @returns  none
*/
/**************************************************************************/
void Trig2Callback() {

//  task code
}

void Trig3Callback() {

//  task code
}




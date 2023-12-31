/*
 * TimeRTCSet.pde
 * example code illustrating Time library with Real Time Clock.
 *
 * RTC clock is set in response to serial port time message 
 * A Processing example sketch to set the time is included in the download
 * On Linux, you can use "date +T%s > /dev/ttyACM0" (UTC time zone)
 */

#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t

const int ledPin = 22;  // the number of the LED pin

long int lightOnTimestamp = 0;
long int lightOffTimestamp = 0;
long int currentTimestamp = 0;

int currentTimestampIndex = 0;

const long int ephemeride[6]={1696695790,
1696695800,
1696695805,
1696695815,
1696695820,
1696695830};

bool lightOn = false;


void setup()  {
  Serial.begin(9600);
  setTime(16, 23, 00, 7, 10, 2023);
  Teensy3Clock.set(now());
  while (!Serial) ; // Needed for Leonardo only
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if (timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");      

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);

  currentTimestamp = ephemeride[currentTimestampIndex];

  // Serial.print("Now: ");
  // Serial.println(now());
  // Serial.print("CurrentTS: ");
  // Serial.println(currentTimestamp);

}

void loop()
{
  if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t != 0) {
      RTC.set(t);   // set the RTC and the system time to the received value
      setTime(t);          
    }
  }

  // if (lightOnTimestamp == 0 && lightOffTimestamp == 0) {
  //   lightOnTimestamp = now() + 10;
  //   lightOffTimestamp = now() + 15;
  // }
  // Serial.println(now());
  // if ((lightOnTimestamp-2 < now()) && (now() < lightOnTimestamp+2){
  if ((currentTimestamp - 1 <= now()) && (now() <= currentTimestamp + 1)){

    if (!lightOn){
    // Serial.println("Light On");
      digitalWrite(ledPin, HIGH);
      currentTimestampIndex += 1;
      currentTimestamp = ephemeride[currentTimestampIndex];
      lightOn = true;
    }
    // digitalClockDisplay();  
    // delay(1000);
    else {  
      digitalWrite(ledPin, LOW);
      currentTimestampIndex += 1;
      currentTimestamp = ephemeride[currentTimestampIndex];
      lightOn = false;
    // Serial.println("Light OFF");
    }
  }
}

void setTime(int hr,int min,int sec,int day, int month, int yr);

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.print(" | ");
  Serial.print(now());
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  return pctime;
}






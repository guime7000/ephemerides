#include <TimeLib.h>
#include <RTClib.h>

RTC_DS3231 rtc;
tmElements_t my_time;  // time elements structure
time_t unix_timestamp; // a timestamp

void setup() {
  Serial.begin(57600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.unixtime());
  Serial.print(" | ");
  Serial.print(now.day(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC);
  Serial.print("  |  ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println(); 
  delay(1000);
  
}



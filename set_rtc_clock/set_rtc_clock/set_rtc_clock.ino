// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

void setup () {
  Serial.begin(57600);

#ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
  }

  // Pour réinitilaiser l'horloge RTC :
  // Modifier la ligne ci-dessous 
  // par ex pour régler l'horloge au 29 mars 2024, 18h 09 min 45 secs
  // écrire rtc.adjust(DateTime(2024, 3, 29, 18, 9, 45));

  // téléverser le code 6 secondes avant l'heure prévue.
  
  rtc.adjust(DateTime(2024, 3, 29, 18, 9, 45));

}

void loop () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    // Serial.print(" since midnight 1/1/1970 = ");
    // Serial.print(now.unixtime());
    // Serial.print("s = ");
    // Serial.print(now.unixtime() / 86400L);
    // Serial.println("d");

    // calculate a date which is 7 days, 12 hours, 30 minutes, 6 seconds into the future
    // DateTime future (now + TimeSpan(7,12,30,6));

    // Serial.print(" now + 7d + 12h + 30m + 6s: ");
    // Serial.print(future.year(), DEC);
    // Serial.print('/');
    // Serial.print(future.month(), DEC);
    // Serial.print('/');
    // Serial.print(future.day(), DEC);
    // Serial.print(' ');
    // Serial.print(future.hour(), DEC);
    // Serial.print(':');
    // Serial.print(future.minute(), DEC);
    // Serial.print(':');
    // Serial.print(future.second(), DEC);
    // Serial.println();

    // Serial.print("Temperature: ");
    // Serial.print(rtc.getTemperature());
    // Serial.println(" C");

    // Serial.println();

    // Serial.print(" since midnight 1/1/1970 = ");
    // Serial.print(now.unixtime());
    // Serial.print("s = ");
    // Serial.print(now.unixtime() / 86400L);
    // Serial.println("d");
    // Serial.println();

    delay(1000);
}

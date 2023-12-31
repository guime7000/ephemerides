#include <TimeLib.h>

// Affectation des broches de LED témoin
const int fake_Relay_1 = 4; // Led Relais 1 sur PIN 4
const int fake_Relay_2 = 5; // Led Relais 2 sur PIN 5
const int fake_Relay_3 = 6; // Led Relais 3 sur PIN 6
const int fake_Relay_4 = 7; // Led Relais 4 sur PIN 7

int relay1_index = 0;
int relay2_index = 0;
int relay3_index = 0;
int relay4_index = 0;

// Création des tableaux de tops

// Relay 1
long int relay1_start_timestamps[8];
long int relay1_stop_timestamps[8];
// Relay 2
long int relay2_start_timestamps[5];
long int relay2_stop_timestamps[5];
// Relay 3
long int relay3_start_timestamps[4];
long int relay3_stop_timestamps[4];
// Relay 4
long int relay4_start_timestamps[3];
long int relay4_stop_timestamps[3];

const int time_offset = 5;  //Time offset for first enlightment after arduino start

long int currentTimestamp = 0;

int currentTimestampIndex = 0;

void setup() {
  Serial.begin(9600);

  const char compile_date[] = __DATE__ " " __TIME__;
  Serial.println(compile_date);


  Serial.print("Now: ");
  Serial.println(now());
  Serial.print("CurrentTS: ");
  Serial.println(currentTimestamp);

  pinMode(fake_Relay_1, OUTPUT);
  pinMode(fake_Relay_2, OUTPUT);
  pinMode(fake_Relay_3, OUTPUT);
  pinMode(fake_Relay_4, OUTPUT);

  const long int my_now = now();

  // Initialisation des tableaux de timestamp
  // Relay 1
  for (int i = 0; i < 8; i++) {
    relay1_start_timestamps[i] = my_now + time_offset + 2 * i;
    relay1_stop_timestamps[i] = my_now + time_offset + 2 * i + 1;
  }
  // Relay 2
  for (int i = 0; i < 5; i++) {
    relay2_start_timestamps[i] = my_now + time_offset + 3 * i;
    relay2_stop_timestamps[i] = relay2_start_timestamps[i] + 2;
  }

  // Relay 3
  for (int i = 0; i < 4; i++) {
    relay3_start_timestamps[i] = my_now + time_offset + 1 + 4 * i;
    relay3_stop_timestamps[i] = relay3_start_timestamps[i] + 3;
  }

  // Relay 4
  relay4_start_timestamps[0] = my_now + time_offset + 2;
  relay4_start_timestamps[1] = my_now + time_offset + 8;
  relay4_start_timestamps[2] = my_now + time_offset + 14;

  relay4_stop_timestamps[0] = my_now + time_offset + 6;
  relay4_stop_timestamps[1] = my_now + time_offset + 13;
  relay4_stop_timestamps[2] = my_now + time_offset + 15;
}

void loop() {
  // Relay 1
  relay1_index = manage_relay(relay1_start_timestamps, relay1_stop_timestamps, fake_Relay_1, relay1_index);

  // Relay 2
  relay2_index = manage_relay(relay2_start_timestamps, relay2_stop_timestamps, fake_Relay_2, relay2_index);

  // Relay 3
  relay3_index = manage_relay(relay3_start_timestamps, relay3_stop_timestamps, fake_Relay_3, relay3_index);

  // Relay 4
  relay4_index = manage_relay(relay4_start_timestamps, relay4_stop_timestamps, fake_Relay_4, relay4_index);

}

int manage_relay(long int start_timestamps[], long int stop_timestamps[], int relay_number, int current_index)
  {
    if ((start_timestamps[current_index] - 1 <= now()) && (now() <= start_timestamps[current_index] + 1)) {
      digitalWrite(relay_number, HIGH);
    }
    

    if ((stop_timestamps[current_index] - 1 <= now()) && (now() <= stop_timestamps[current_index] + 1)) {
      digitalWrite(relay_number, LOW);
      current_index += 1;
    }
    return current_index;
  }

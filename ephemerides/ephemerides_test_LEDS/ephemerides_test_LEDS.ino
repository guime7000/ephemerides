#include <TimeLib.h>
#include <RTClib.h>

RTC_DS3231 rtc;



// Affectation des broches de LED témoin
const byte motor_relay_1 = 4; // Motor Relais 1 sur PIN 4
const byte led_relay_1 = 5; // Led Relais 2 sur PIN 5
const byte led_relay_2 = 6; // Led Relais 3 sur PIN 6
const byte motor_relay_2 = 7; // Motor Relais 4 sur PIN 7

// unsigned long start_ts[] = {0,0,0,0};
unsigned long start_ts[] = {0,0,0,0};

int start_timestamp_index = 0;

unsigned long current_timestamp;
unsigned int timestamp_step;

byte pwm_command[512];

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

  pinMode(motor_relay_1, OUTPUT);
  pinMode(led_relay_1, OUTPUT);
  pinMode(led_relay_2, OUTPUT);
  pinMode(motor_relay_2, OUTPUT);

  Serial.println("***** Waiting for 3 secs to find Real Time CLock... *****");
  delay(3000);

  unsigned int attack_duration = 15; // Durée (en s) de la montée pour les leds
  const int number_of_starts = 3; // Nombre de timestamps de départ
  start_ts[0] = rtc.now().unixtime() + 5; // La première montée démarre 5 secondes après le démarrage du programme principal
  start_ts[1] = rtc.now().unixtime() + 10 + 2* attack_duration; // Après 15 secondes de montée LEDS puis 15 secondes de Descente LEDS, ça repart 5 secondes après l'extinction des leds.
  start_ts[2] = rtc.now().unixtime() + 15 + 2* attack_duration; // Après 15 secondes de montée LEDS puis 15 secondes de Descente LEDS, ça repart 5 secondes après l'extinction des leds.
  start_ts[3] = 0;

  byte pwm_step = compute_pwm_steps(attack_duration);
  timestamp_step = compute_timestamp_step(attack_duration);

  Serial.println("***** RTC Found ! Let's go !");
  Serial.println("Actual Unix timestamp: " + rtc.now().unixtime());
  Serial.println(" Timestamp step (in secs): " + timestamp_step);
  Serial.println(" PWM command step increment: " + pwm_step);

  if (attack_duration > 256){
    for (int i=0; i < 256; i++){
      pwm_command[i] = i* pwm_step;
    }
    for (int i = 256; i<512; i++ ){
      pwm_command[i] = pwm_command[i-1] - pwm_step;
    }
  }
  else{
    for (int i=0; i < attack_duration; i++){
      pwm_command[i] = i* pwm_step;
    }
    for (int i = attack_duration; i<2*attack_duration; i++ ){
      pwm_command[i] = pwm_command[i-1] - pwm_step;
    }
  }

}

void loop(){
  DateTime now = rtc.now();
  int led_timestamp_index = 0;
  unsigned int steps_limit= 512;
  if (attack_duration<256){
    steps_limit = 2* attack_duration;
  }

  if (is_valid_timestamp(start_ts[start_timestamp_index])){
    current_timestamp = start_ts[start_timestamp_index];
    power_motor(motor_relay_1);
    power_motor(motor_relay_2);
    while (led_timestamp_index < steps_limit){
      if (is_valid_timestamp(current_timestamp)){
        analogWrite(led_relay_1, pwm_command[led_timestamp_index]);
        analogWrite(led_relay_2, pwm_command[led_timestamp_index]);
        current_timestamp += timestamp_step;
        led_timestamp_index +=1;
      }
    }
    stop_motor(motor_relay_1);
    stop_motor(motor_relay_2);
    analogWrite(led_relay_1, 0);
    analogWrite(led_relay_2, 0);
    start_timestamp_index += 1;
  }
  

}

bool is_valid_timestamp(unsigned long timestamp){
  if ((timestamp - 1 <= rtc.now().unixtime()) && (rtc.now().unixtime() <= timestamp + 1)){
    return true;
  }
  return false;
}

unsigned int compute_timestamp_step(unsigned int attack_duration)  {
    if (attack_duration < 256){
      return 1;
    }
    return floor(attack_duration/256);
  }

byte compute_pwm_steps(unsigned int attack_duration){
  if (attack_duration < 256){
      return floor(256 / attack_duration);
    }
    return 1;
  }

void power_motor(int relay_number){
      digitalWrite(relay_number, HIGH);
    }    

void stop_motor(int relay_number){
    digitalWrite(relay_number, LOW);
  }


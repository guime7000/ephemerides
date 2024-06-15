#include <TimeLib.h>
#include <RTClib.h>

RTC_DS3231 rtc;

// ============= Paramètrage données Installation =========================

// Ajouter les timestamps (la date en secondes en dessous. Attention aux virgules !!)
unsigned long start_ts[] = {
1718488170,
};

// Durée (en s) de la montée pour les leds
unsigned int attack_duration = 1200;

// Durée (en s) du climax une fois les leds allumées à fond après la montée
unsigned int climax_duration = 60; 

// ============= Fin Paramètrage données Installation =========================

// Affectation des broches de LED témoin
const byte motor_relay_1 = 4; // Motor Relais 1 sur PIN 4
const byte led_relay_1 = 5; // Led Relais 2 sur PIN 5
const byte led_relay_2 = 6; // Led Relais 3 sur PIN 6
const byte motor_relay_2 = 7; // Motor Relais 4 sur PIN 7

// Paramétrage Check Sequence
const byte check_button=2; // Pin for check sequence push button
bool check_button_state = 0;
const byte check_green_led=10; // Green Led for checks
const byte check_red_led=11; // Red Led for checks

// Paramétrage Display RTC
const byte check_button_rtc=8; // Pin for display RTC push button
bool check_button_rtc_state = 0;

// Paramétrage Déclenchement séquence réelle
const byte check_button_seq=12; // Pin for real sequence push button trigger
bool check_button_seq_state = 0;


int start_timestamp_index = 0;
unsigned long current_timestamp;
unsigned int timestamp_step;
byte nb_iter;
byte iteration_counter = 0;
byte pwm_command = 0;
byte pwm_step;

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

  pinMode(check_button, INPUT_PULLUP);
  pinMode(check_green_led, OUTPUT);
  pinMode(check_red_led, OUTPUT);

  pinMode(check_button_rtc, INPUT_PULLUP);

  pinMode(check_button_seq, INPUT_PULLUP);

  delay(3000);

  pwm_step = compute_pwm_steps(attack_duration);
  timestamp_step = compute_timestamp_step(attack_duration);
  nb_iter = compute_nb_iter(attack_duration);

  DateTime init_now = rtc.now();
  
}

void loop(){

  DateTime now = rtc.now();
  int led_timestamp_index = 0;

  check_button_state = digitalRead(check_button);
  if (!check_button_state){
    check_button_sequence();
    }

  check_button_rtc_state = digitalRead(check_button_rtc);
  if (!check_button_rtc_state){
    display_RTC();
    }

  check_button_seq_state = digitalRead(check_button_seq);
  if (!check_button_seq_state){
    power_motor(motor_relay_1);
    power_motor(motor_relay_2);
    
    led_power_management(rtc.now().unixtime(), nb_iter, 1); // Montée lumineuse de durée attack_duration

    analogWrite(check_green_led, 255);
    analogWrite(check_red_led, 255);
    
    delay(climax_duration*1000); // Climax de durée climax_duration secondes;
    
    led_power_management(rtc.now().unixtime(), nb_iter, 0); // Descente lumineuse de durée attack_duration
    
    analogWrite(check_red_led, 0);

    stop_motor(motor_relay_1);
    stop_motor(motor_relay_2);
    analogWrite(led_relay_1, 0);
    analogWrite(led_relay_2, 0);
    }  

  if (is_valid_timestamp(start_ts[start_timestamp_index])){
    current_timestamp = start_ts[start_timestamp_index];
    power_motor(motor_relay_1);
    power_motor(motor_relay_2);

    led_power_management(current_timestamp, nb_iter, 1); // Montée lumineuse de durée attack_duration

    analogWrite(check_green_led, 255);
    analogWrite(check_red_led, 255);

    delay(climax_duration*1000); // Climax de durée climax_duration secondes;
    
    analogWrite(check_green_led, 0);
    analogWrite(check_red_led, 0);

    led_power_management(rtc.now().unixtime(), nb_iter, 0); // Descente lumineuse de durée attack_duration

    stop_motor(motor_relay_1);
    stop_motor(motor_relay_2);
    analogWrite(check_green_led, 0);
    analogWrite(check_red_led, 0);
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
      return ceil(256 / attack_duration);
    }
    return 1;
  }

byte compute_nb_iter(unsigned int attack_duration){
  if (attack_duration < 256) {
    return attack_duration;
  }
  return 255;
}

void led_power_management(unsigned long current_timestamp, byte nb_iter_max, byte mode){
  // mode = 1 : la puissance augmente
  // mode = 0 : la puissance diminue

  byte count_iter = 0;
  // if (mode == 0){
  //   current_timestamp = current_timestamp + climax_duration;
  // }
  while (count_iter < nb_iter_max){
      if (is_valid_timestamp(current_timestamp)){
        analogWrite(led_relay_1, pwm_command);
        analogWrite(led_relay_2, pwm_command);
        current_timestamp += timestamp_step;
        if (mode == 1){
          pwm_command += pwm_step;
          analogWrite(check_green_led, 255);
          analogWrite(check_red_led, 0);
        }
        else{
          pwm_command -= pwm_step;
          analogWrite(check_green_led, 0);
          analogWrite(check_red_led, 255);
        }
        count_iter += 1;
      }
    }

}

void power_motor(int relay_number){
      digitalWrite(relay_number, HIGH);
    }    

void stop_motor(int relay_number){
    digitalWrite(relay_number, LOW);
  }

// Gestion de la séquence d'allumage des diodes de test.
void check_button_sequence(){

  int duree_palier = 1000; // durée des paliers en millisecondes

  analogWrite(check_green_led, 0);
  analogWrite(check_red_led, 0);
  analogWrite(led_relay_1, 0);
  analogWrite(led_relay_2, 0);

  analogWrite(check_green_led, 255);
  analogWrite(led_relay_1, 63);
  analogWrite(led_relay_2, 63);
  delay(duree_palier);

  analogWrite(check_green_led, 255);
  analogWrite(led_relay_1, 128);
  analogWrite(led_relay_2, 128);
  delay(duree_palier);
  
  analogWrite(check_green_led, 255);
  analogWrite(check_red_led, 255);
  analogWrite(led_relay_1, 255);
  analogWrite(led_relay_2, 255);
  delay(duree_palier * 3);

  analogWrite(check_green_led, 0);
  analogWrite(check_red_led, 255);
  analogWrite(led_relay_1, 128);
  analogWrite(led_relay_2, 128);
  delay(duree_palier);

  analogWrite(check_red_led, 255);
  analogWrite(led_relay_1, 63);
  analogWrite(led_relay_2, 63);
  delay(duree_palier);

  analogWrite(check_red_led, 0);
  analogWrite(led_relay_1, 0);
  analogWrite(led_relay_2, 0);
  delay(duree_palier);  
}

void display_RTC(){
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

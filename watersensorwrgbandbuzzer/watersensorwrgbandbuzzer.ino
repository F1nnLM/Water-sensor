
#include "pitches.h"
#define BLUE 3
#define GREEN 5
#define RED 6
#define SENSOR_PIN 0

int button_pin = 2;
int button_status = LOW;
int redValue;
int greenValue;
int blueValue;
bool note = false;

unsigned long currentTime = 0;
bool set = false;
unsigned long lastUpdateTime = 0;

int melody[] = {
  NOTE_A5, NOTE_D5,};
int duration = 500;

enum State {
  FULL,
  EMPTY,
  OFF,
  ON
};

State water_level = FULL;
State board_state = OFF;
State buzzer_state = OFF;
int last_button = LOW;
int current_button = LOW;

void setup()
{
pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
pinMode(button_pin, INPUT);
Serial.begin(9600);
}

void loop()
{
  current_button = digitalRead(button_pin);

  if(board_state == ON){

    currentTime = millis();

    int level = analogRead(SENSOR_PIN);

    if (water_level == FULL && level <= 300) {
      // critical level
      water_level = EMPTY;
      redValue = 255;
      RGB();
      buzzer_state = ON;
    } else if (water_level == EMPTY && level >= 400){
      note = false;
      water_level = FULL;
      if(set == true){
        redValue = 0;
        RGB();
        set = false;
      }
      buzzer_state = OFF;
    }

    if(buzzer_state == ON){
      buzz();
    }

    if (current_button == HIGH && last_button == LOW) {
      redValue = 0;
      RGB();
      //buzz_state = OFF;
      board_state = OFF;
      delay(200); 
      last_button = current_button;
    }
  }
  else {
    if (current_button == HIGH && last_button == LOW) {
      board_state = ON;
      delay(200); 
    }
  }

  last_button = current_button;
}



void RGB() {
  //red
  digitalWrite(RED, redValue);
  //green
  digitalWrite(GREEN, greenValue);
  // blue
  digitalWrite(BLUE, blueValue);
}


void buzz() {
  if(set == false){
    lastUpdateTime = currentTime;
    set = true;
    tone(12, melody[(note == false)? 0:1], duration);
    note = !note;
  }

  if(currentTime - lastUpdateTime >= 500){
    lastUpdateTime = currentTime;
    tone(12, melody[(note == false)? 0:1], duration);
    note = !note;
  }
}





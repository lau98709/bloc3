#include "SerialCmd.h"
#include "FString.h"
#include "Stepper.h"

#define LED_PIN 13
#define STEPPER_PIN1 32
#define STEPPER_PIN2 33
#define STEPPER_PIN3 25
#define STEPPER_PIN4 26
#define FMAX_PIN 2
#define FMIN_PIN 15
#define BTN1_PIN 36
#define BTN2_PIN 39

Stepper4P *stepper;

int dir;
long dt;
int led;
long t,t2;

void Traitement(char *cmd) {
  FString s(cmd);
  s.ToUpper();
  if (s == "F") {
    dir = 1;
    Serial.println("FORWARD");
  }
  if (s == "R") {
    dir = -1;
    Serial.println("BACKWARD");
  }
  if (s == "S") {
    dir = 0;
    Serial.println("STOP");
  }
  if (s.BeginWith("D")) {
    dt = s.GetInt(2);
    Serial.print("DELAY=");
    Serial.println(dt);
  }
  if (s == "L") {
    led = (led == 0)? 1 : 0;
    Serial.print("LED=");
    Serial.println(led);
  }
}


void setup() {  
  SerialCmd.begin(115200,Traitement);
  dir = 1;
  dt = 10;
  stepper = new Stepper4P(STEPPER_PIN1,STEPPER_PIN2,STEPPER_PIN3,STEPPER_PIN4,16);

  led = 1;
  t = t2 = millis();

  pinMode(FMAX_PIN,INPUT);      // fin de course
  pinMode(FMIN_PIN,INPUT);      // fin de course

  pinMode(LED_PIN,OUTPUT);     // LED verte
  pinMode(BTN1_PIN,INPUT);      // bouton
  pinMode(BTN2_PIN,INPUT);       // bouton

  digitalWrite(LED_PIN,HIGH);
}


void loop() {
  SerialCmd.run();

  if (dir != 0) {
    digitalWrite(LED_PIN,((millis() % 1000) > 500));
  } else {
    digitalWrite(LED_PIN,LOW);
  }

  if (millis() > t2) {
    if ((dir != 1) && (digitalRead(BTN1_PIN) == HIGH)) {
      Serial.println("FORWARD");
      dir = 1;
    }
    if ((dir != -1) && (digitalRead(BTN2_PIN) == HIGH)) {
      Serial.println("BACKWARD");
      dir = -1;
    }
    if ((dir != 0) && (digitalRead(BTN1_PIN) == HIGH) && (digitalRead(BTN2_PIN)==HIGH)) {
      Serial.println("STOP");
      dir = 0;
    }
    t2 = millis() + 200;
  }

  if ((dir == -1) && (digitalRead(FMAX_PIN)==HIGH)) dir = 0;
  if ((dir == 1) && (digitalRead(FMIN_PIN)==LOW)) dir = 0;

  if (dir != 0) stepper->Step(dir == 1);
  delayMicroseconds(((long)dt)*100);

  if (millis() > t) {
    char txt[32];
    sprintf(txt,"dir=%d %d %d b1=%d b2=%d",
        dir, 
        digitalRead(FMIN_PIN), digitalRead(FMAX_PIN),
        digitalRead(BTN1_PIN), digitalRead(BTN2_PIN));
    Serial.println(txt);
    t = millis() + 500;
  }
}


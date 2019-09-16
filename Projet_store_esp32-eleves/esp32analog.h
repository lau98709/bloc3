#ifndef ESP32ANALOG_H
#define ESP32ANALOG_H

#define ESP32

#define MAX_PWM_PIN 40
#define MAX_PWM_CHANNELS 16

int pwm_channels[MAX_PWM_PIN] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

void InitAnalogPin( int pin ) {
  int i, j;
  
  for (i=0; i<MAX_PWM_CHANNELS; i++) {
    for (j=0; j<MAX_PWM_PIN; j++) {
      if (pwm_channels[j] == i) break;         
    }
    if (j >= MAX_PWM_PIN) {
      pwm_channels[pin] = i;
      ledcSetup(pwm_channels[pin], 2000, 8);
      ledcAttachPin(pin, pwm_channels[pin]);
      return;
    }
  }
}

void analogWrite( int pin, int value ) {
  ledcWrite(pwm_channels[pin], value);
}

#endif



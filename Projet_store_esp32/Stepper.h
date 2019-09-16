#ifndef STEPPER_H
#define STEPPER_H

#include <math.h>
#include "esp32analog.h"

// Bipolar stepper motor driving
// By applying sinewave to coils
// Microstepping with arbitrary number of steps
// Requires 4 PWM pins
//
// Usage :
// 1) Define variable
//      Stepper *stepper;
// 2) In setup, init "stepper" by giving pins number
//    (must be PWM pins) and number of steps
//      stepper = new Stepper(i1,i2,i3,i4,N);
// 3) In loop, move motor with Step()
//      stepper->Step(true);
//

class Stepper {
  public:
  int pin[4];
  boolean state[4];
  int nsteps,current_step;
  double alpha;

  // Constructor
  //  i1,i2,i3,i4 pins number
  //  N = number of steps, 4 = full step, 8 = half step, > 8 microstepping
  Stepper( int i1, int i2, int i3, int i4, int N ) {
    pin[0] = i1;
    pin[1] = i2;
    pin[2] = i3;
    pin[3] = i4;
    for (int i=0; i<4; i++) {
#ifdef ESP32
      InitAnalogPin(pin[i]);
#else
      pinMode(pin[i],OUTPUT);
      digitalWrite(pin[i],LOW);
#endif      
    }
    alpha = 1;
    ChangeSteps(N);
    GoTo(current_step);
  }

  // Change number of steps
  virtual void ChangeSteps( int N ) {
    nsteps = N;
    current_step = 0;
  }

  virtual void SetAlpha( double a ) {
    alpha = a;
  }

  virtual double ACorrect( double x ) {
    double a, b, c;
    a = 4*(1-alpha);
    b = 6*(alpha-1);
    c = 3-2*alpha;
    return x*(c+x*(b+a*x));
  }

  // Move to a substep
  virtual void GoTo( int istep ) {
    //double a = ((double)istep)*2*PI/nsteps;
    double a = ACorrect((double)istep/nsteps)*2*PI;
    double x = cos(a)*255;
    double y = sin(a)*255;
    if (x >= 0) {
      analogWrite(pin[0],(int)(x-0.5));
      digitalWrite(pin[1],LOW);
    } else {
      digitalWrite(pin[0],LOW);
      analogWrite(pin[1],(int)(-x-0.5));
    }
    if (y >= 0) {
      analogWrite(pin[2],(int)(y-0.5));
      digitalWrite(pin[3],LOW);
    } else {
      digitalWrite(pin[2],LOW);
      analogWrite(pin[3],(int)(-y-0.5));
    }
  }

  // Get pins state
  virtual void ReadState() {
    for (int i=0; i<4; i++) {
      state[i]=digitalRead(pin[i]);
    }
  }  

  // Move on step
  virtual void Step( boolean forward ) {
    int k = (forward)? current_step+1 : current_step+nsteps-1;
    current_step = (k % nsteps);
    GoTo(current_step);
  }
}; 


// 4 phases stepper motor driving
// By applying sinewave to coils
// Microstepping with arbitrary number of steps
class Stepper4P : public Stepper {
  public:
  Stepper4P( int i1, int i2, int i3, int i4, int N ) :
    Stepper(i1,i2,i3,i4,N) {
  }
  
  // Sub function to write voltage to pin
  void OutputPhase( int outpin, double x ) {
    if (x >= 0) {
      analogWrite(outpin,(int)(x-0.5));
    } else {
      analogWrite(outpin,(int)(-x-0.5));
    }
  }

  // Move to a substep
  virtual void GoTo( int istep ) {
    double a = ACorrect((double)istep/nsteps)*2*PI;
    double x = (0.5+0.5*cos(a))*255;
    OutputPhase(pin[0],x);
    x = (0.5+0.5*cos(a+PI/2))*255;
    OutputPhase(pin[1],x);
    x = (0.5+0.5*cos(a+PI))*255;
    OutputPhase(pin[2],x);
    x = (0.5+0.5*cos(a+3*PI/2))*255;
    OutputPhase(pin[3],x);
  }
};

#endif


#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

class State {

public:
  virtual void step(unsigned long dt) {}
  virtual void onKeyPress(int btn, unsigned long duration, boolean repeated) {}
  virtual void onKeyRelease(int btn, unsigned long duration) {}
  virtual void onKeyDown(int btn) {}
  virtual void onKeyUp(int btn) {}

};

#endif



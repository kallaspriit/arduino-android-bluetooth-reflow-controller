#ifndef STATE_H
#define STATE_H

#include "Arduino.h"

class State {

public:
  virtual int step(float dt) {}
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void onKeyPress(int btn, unsigned long duration, boolean repeated) {}
  virtual void onKeyRelease(int btn, unsigned long duration) {}
  virtual void onKeyDown(int btn) {}
  virtual void onKeyUp(int btn) {}
  
  virtual void setIntent(int intent);
  virtual int popLastIntent();
  
protected:
  int intent;

};

#endif



#ifndef STATE_H
#define STATE_H

#include "Arduino.h"
#include "Config.h"

class aJsonObject;

class State {

public:
  virtual const char* getName() = 0;
  virtual int step(float dt) { return INTENT_NONE; }
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void onKeyPress(int btn, unsigned long duration, boolean repeated) {}
  virtual void onKeyRelease(int btn, unsigned long duration) {}
  virtual void onKeyDown(int btn) {}
  virtual void onKeyUp(int btn) {}
  
  virtual aJsonObject* getStateInfo() { return NULL; };
  
  virtual void setIntent(int intent);
  virtual int popLastIntent();
  
protected:
  void sendStateInfo();

  int intent;

};

#endif



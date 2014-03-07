#ifndef REFLOWSTATE_H
#define REFLOWSTATE_H

#include "State.h"

#include "Arduino.h"

class Adafruit_PCD8544;
class Menu;

class ReflowState : public State {

public:
  ReflowState(Adafruit_PCD8544* display);
  
  int step(float dt);
  void onKeyPress(int btn, unsigned long duration, boolean repeated);
  
private:
  Adafruit_PCD8544* display;

};

#endif



#ifndef CONFIGUREPIDSTATE_H
#define CONFIGUREPIDSTATE_H

#include "State.h"
#include "Renderer.h"

#include "Arduino.h"

class Adafruit_PCD8544;
class PID;

class ConfigurePidState : public State {

public:
  ConfigurePidState(Adafruit_PCD8544* display, PID* pid);
  
  const char* getName() { return "configure-pid"; }
  int step(float dt);
  void onKeyPress(int btn, unsigned long duration, boolean repeated);
  
private:
  Adafruit_PCD8544* display;
  PID* pid;
  Renderer renderer;

};

#endif



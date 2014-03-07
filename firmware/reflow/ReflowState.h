#ifndef REFLOWSTATE_H
#define REFLOWSTATE_H

#include <Arduino.h>

#include "State.h"
#include "ProfileRenderer.h"
#include "Config.h"

class Adafruit_PCD8544;
class Owen;
class Menu;

class ReflowState : public State {

public:
  ReflowState(Adafruit_PCD8544* display, Owen* owen, ReflowProfile* profile);
  
  int step(float dt);
  void onEnter();
  void onExit();
  void onKeyPress(int btn, unsigned long duration, boolean repeated);
  
private:
  void renderTemperatures(int sensorTemp, int targetTemp);
  void renderProgress(int percentage, int duration);

  Adafruit_PCD8544* display;
  Owen* owen;
  ReflowProfile* profile;
  ProfileRenderer profileRenderer;
  
  float reflowDuration;
  boolean reflowing;
  unsigned long lastRenderTime;
  int realTemperatures[PROFILE_MAX_DURATION];

};

#endif



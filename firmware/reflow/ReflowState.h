#ifndef REFLOWSTATE_H
#define REFLOWSTATE_H

#include <Arduino.h>

#include "State.h"
#include "Renderer.h"
#include "Config.h"

class Adafruit_PCD8544;
class Owen;
class Menu;

class ReflowState : public State {

public:
  struct StateInfo {
    float duration;
    float currentTemperature;
    float targetTemperature;
    boolean reflowing;
  };

  ReflowState(Adafruit_PCD8544* display, Owen* owen, ReflowProfile* profile);
  
  const char* getName() { return "reflow"; }
  int step(float dt);
  void onEnter();
  void onExit();
  void onKeyPress(int btn, unsigned long duration, boolean repeated);
  
  // TODO restore
  //aJsonObject* getStateInfo();
  
private:
  void renderTemperatures(int sensorTemp, int targetTemp);
  void renderProgress(int percentage, int duration);
  void renderConfirmExit(float timeout);

  Adafruit_PCD8544* display;
  Owen* owen;
  ReflowProfile* profile;
  Renderer renderer;
  StateInfo stateInfo;
  
  float reflowDuration;
  boolean reflowing;
  unsigned long lastRenderTime;
  int realTemperatures[PROFILE_MAX_DURATION];
  float confirmExitTimeout;
  int profileInfoIndex;
  int profileInfoCount;
  float profileInfoChangeInterval;
  float profileInfoChangeTimeout;

};

#endif



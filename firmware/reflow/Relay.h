#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay {

public:
  Relay(int pin, int connectedLevel = LOW, float minSwitchingPause = 1.0f);
  
  void setConnected(boolean isConnected, boolean force = false);
  boolean isConnected();
  void reset();
  
  void step(float dt);
  
private:
  int pin;
  int connectedLevel;
  boolean currentState;
  boolean requestedState;
  float minSwitchingPause;
  float timeSinceLastSwitch;

};

#endif



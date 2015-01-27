#include "Relay.h"
#include "Config.h"

Relay::Relay(int pin, int connectedLevel, float minSwitchingPause) :
  pin(pin),
  connectedLevel(connectedLevel),
  minSwitchingPause(minSwitchingPause),
  timeSinceLastSwitch(minSwitchingPause),
  currentState(false),
  requestedState(false)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, connectedLevel == HIGH ? LOW : HIGH);
  
  setConnected(false);
}
  
void Relay::setConnected(boolean mode, boolean force) {
  requestedState = mode;
  
  if (force) {
    currentState = requestedState;
    timeSinceLastSwitch = minSwitchingPause;
    digitalWrite(pin, currentState ? connectedLevel : connectedLevel == HIGH ? LOW : HIGH);
  }
}

boolean Relay::isConnected() {
  return currentState;
}

void Relay::reset() {
  setConnected(false, true);
  timeSinceLastSwitch = minSwitchingPause;
}

void Relay::step(float dt) {
  /*COMM.print("currentState: ");
  COMM.print(currentState ? "on" : "off");
  COMM.print(", requested: ");
  COMM.print(requestedState ? "on" : "off");
  COMM.print(", since last: ");
  COMM.print(timeSinceLastSwitch);
  COMM.print(", pause: ");
  COMM.print(minSwitchingPause);
  COMM.println();
  delay(100);*/
  
  if (timeSinceLastSwitch < minSwitchingPause) {
     timeSinceLastSwitch += dt;
  } else {
    if (currentState == requestedState) {
      return;
    }
    
    currentState = requestedState;
    
    digitalWrite(pin, currentState ? connectedLevel : connectedLevel == HIGH ? LOW : HIGH);
    
    timeSinceLastSwitch = 0.0f;
  }
}

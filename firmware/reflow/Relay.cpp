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
  /*SERIAL.print("currentState: ");
  SERIAL.print(currentState ? "on" : "off");
  SERIAL.print(", requested: ");
  SERIAL.print(requestedState ? "on" : "off");
  SERIAL.print(", since last: ");
  SERIAL.print(timeSinceLastSwitch);
  SERIAL.print(", pause: ");
  SERIAL.print(minSwitchingPause);
  SERIAL.println();
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

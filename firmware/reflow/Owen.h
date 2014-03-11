#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>

#include "Relay.h"
#include "../Adafruit_MAX31855/Adafruit_MAX31855.h"

class Owen {

public:
  Owen();
  
  int getTemperature();
  
  void setEnabled(boolean enabled);
  void setHeaterOn(boolean enabled);
  void setSimulationMode(boolean enabled);
  void setTargetTemperature(int temperature);
  void reset();
  
  void step(float dt);
  
private:
  void simulate(float dt);

  Adafruit_MAX31855 thermocouple;
  Relay relay;
  boolean enabled;
  boolean heaterOn;
  int targetTemperature;
  
  boolean simulationMode;
  float simulatedTemperature;
  float simulatedHeaterSpeed;

};

#endif



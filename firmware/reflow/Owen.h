#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>

#include "../Adafruit_MAX31855/Adafruit_MAX31855.h"

class Owen {

public:
  Owen();
  
  int getTemperature();
  
  void setEnabled(boolean enabled);
  void setHeaterOn(boolean enabled);
  void setSimulationMode(boolean enabled);
  void setTargetTemperature(int temperature);
  
  void step(float dt);
  
private:
  Adafruit_MAX31855 thermocouple;
  boolean enabled;
  boolean heaterOn;
  boolean simulationMode;
  int targetTemperature;
  float simulatedTemperature;

};

#endif



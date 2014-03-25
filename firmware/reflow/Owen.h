#ifndef OWEN_H
#define OWEN_H

#include <Arduino.h>

#include "Relay.h"
#include "PID.h"
//#include "../PID_v1/PID_v1.h"
#include "../Adafruit_MAX31855/Adafruit_MAX31855.h"

class Owen {

public:
  Owen();
  
  float getTemperature();
  
  void init();
  void setEnabled(boolean enabled);
  void setHeaterOn(boolean enabled);
  void setSimulationMode(boolean enabled);
  void setTargetTemperature(float temperature);
  void reset();
  PID* getPID();
  
  void step(float dt);
  
private:
  void simulate(float dt);

  Adafruit_MAX31855 thermocouple;
  Relay relay;
  boolean enabled;
  boolean heaterOn;
  float targetTemperature;
  float timeSinceLastControl;
  
  PID pid;
  /*double pidP;
  double pidI;
  double pidD;
  double pidInput;
  double pidOutput;
  double pidSetpoint;
  int pidWindowSize;
  unsigned long pidWindowStartTime;*/
  
  boolean simulationMode;
  float simulatedTemperature;
  float simulatedHeaterSpeed;

};

#endif



#include "Owen.h"
#include "Config.h"

Owen::Owen() : thermocouple(THRERMO_CLK, THERMO_CS, THERMO_DO), enabled(false), simulationMode(false), targetTemperature(0), simulatedTemperature(25.0f) {
  
}

void Owen::setEnabled(boolean enabled) {
  this->enabled = enabled;
}

void Owen::setHeaterOn(boolean enabled) {
  heaterOn = enabled;
}

void Owen::setSimulationMode(boolean enabled) {
  simulationMode = enabled;
  
  if (enabled) {
    simulatedTemperature = 25.0f;
  }
}

void Owen::setTargetTemperature(int temperature) {
  targetTemperature = temperature;
}

void Owen::step(float dt) {
  int temperature = getTemperature();
  
  if (enabled) {
     // todo implement PID
     if (temperature < targetTemperature) {
       setHeaterOn(true);
     } else {
       setHeaterOn(false);
     }
  }
  
  if (!simulationMode) {
    return; 
  }
  
  SERIAL.print("current: ");
  SERIAL.print(temperature);
  SERIAL.print(", target: ");
  SERIAL.print(targetTemperature);
  SERIAL.print(", enabled: ");
  SERIAL.print(enabled ? "yes" : "no");
  SERIAL.print(", simulation: ");
  SERIAL.print(simulationMode ? "yes" : "no");
  SERIAL.print(", heating: ");
  SERIAL.print(heaterOn ? "yes" : "no");
  SERIAL.println();
  delay(100);
  
  if (heaterOn) {
    simulatedTemperature += SIMULATION_HEATING_SPEED * dt;
    
    if (simulatedTemperature > 300) {
      simulatedTemperature = 300;
    }
  } else {
    simulatedTemperature -= SIMULATION_COOLING_SPEED * dt;
    
    if (simulatedTemperature < 25) {
      simulatedTemperature = 25;
    }
  }
}

int Owen::getTemperature() {
  if (simulationMode) {
    return simulatedTemperature;
  } else {
    return thermocouple.readCelsius(); 
  }
}

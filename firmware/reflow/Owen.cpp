#include "Owen.h"
#include "Config.h"

Owen::Owen() : thermocouple(THRERMO_CLK, THERMO_CS, THERMO_DO), relay(RELAY_PIN, RELAY_CONNECTED_LEVEL, RELAY_MIN_INTERVAL), enabled(false), simulationMode(false), targetTemperature(0), simulatedTemperature(20.0f) {
  
}

void Owen::setEnabled(boolean enabled) {
  this->enabled = enabled;
  
  if (!enabled) {
    relay.setConnected(false, true); 
  }
}

void Owen::setHeaterOn(boolean enabled) {
  heaterOn = enabled;
}

void Owen::setSimulationMode(boolean enabled) {
  simulationMode = enabled;
}

void Owen::setTargetTemperature(int temperature) {
  targetTemperature = temperature;
}

void Owen::reset() {
  relay.reset();
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
  
  if (heaterOn) {
    relay.setConnected(true); 
  } else {
    relay.setConnected(false); 
  }
  
  relay.step(dt);
  
  if (!simulationMode) {
    return; 
  }
  
  /*SERIAL.print("current: ");
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
  delay(100);*/
  
  if (relay.isConnected()) {
    simulatedTemperature += SIMULATION_HEATING_SPEED * dt;
    
    if (simulatedTemperature > 300) {
      simulatedTemperature = 300;
    }
  } else {
    simulatedTemperature -= SIMULATION_COOLING_SPEED * dt;
    
    if (simulatedTemperature < 20) {
      simulatedTemperature = 20;
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

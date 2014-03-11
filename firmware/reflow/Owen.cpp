#include "Owen.h"
#include "Config.h"

Owen::Owen() :
  thermocouple(THRERMO_CLK, THERMO_CS, THERMO_DO),
  relay(RELAY_PIN, RELAY_CONNECTED_LEVEL, RELAY_MIN_INTERVAL),
  pid(),
  /*pidP(1.0),
  pidI(0.0),
  pidD(0.0),
  pidWindowSize(3000),
  pid(&pidInput, &pidOutput, &pidSetpoint, pidP, pidI, pidD, DIRECT),*/
  enabled(false),
  targetTemperature(0.0f),
  timeSinceLastControl(0.0f),
  simulationMode(false),
  simulatedTemperature(20.0f),
  simulatedHeaterSpeed(0.0f)
{
  pid.p = 1.0f; // TODO get these from memory
  pid.i = 0.0005f;
  pid.d = 20.0f;
  
  //pid.SetOutputLimits(0, pidWindowSize);
}

void Owen::setEnabled(boolean enabled) {
  this->enabled = enabled;
  
  if (enabled) {
    //pidWindowStartTime = millis();
    //pid.SetMode(AUTOMATIC);
  } else {
    relay.setConnected(false, true); 
  }
}

void Owen::setHeaterOn(boolean enabled) {
  heaterOn = enabled;
}

void Owen::setSimulationMode(boolean enabled) {
  simulationMode = enabled;
}

void Owen::setTargetTemperature(float temperature) {
  targetTemperature = temperature;
}

void Owen::reset() {
  relay.reset();
}

void Owen::step(float dt) {
  float temperature = getTemperature();
  float pidValue = 0.0f;
  
  if (enabled) {
    timeSinceLastControl += dt;

    if (timeSinceLastControl >= 1.0f) {    
      pid.setTarget(targetTemperature);
       
      pidValue = pid.getValue(temperature, timeSinceLastControl);
       
      if (pidValue > 0.0f) {
        setHeaterOn(true);
      } else {
        setHeaterOn(false);
      }
      
      timeSinceLastControl = 0.0f;
      
      //SERIAL.print("Relay connected: ");
      //SERIAL.println(relay.isConnected() ? "yes" : "no");
    }
    
    // todo implement PID
    /*if (temperature < targetTemperature) {
      setHeaterOn(true);
    } else {
      setHeaterOn(false);
    }*/
     
    /*pidInput = temperature;
    pidSetpoint = targetTemperature;
     
    pid.Compute();
     
    if (millis() - pidWindowStartTime > pidWindowSize) { //time to shift the Relay Window
      pidWindowStartTime += pidWindowSize;
    }
    
    if (pidOutput < millis() - pidWindowStartTime) {
      setHeaterOn(true);
    } else {
      setHeaterOn(false);
    }*/
  }
  
  if (heaterOn) {
    relay.setConnected(true); 
  } else {
    relay.setConnected(false); 
  }
  
  relay.step(dt);
  
  if (simulationMode) {
    simulate(dt); 
  }
  
  /*SERIAL.print("current: ");
  SERIAL.print(getTemperature());
  SERIAL.print(", target: ");
  SERIAL.print(targetTemperature);
  SERIAL.print(", speed: ");
  SERIAL.print(simulatedHeaterSpeed);
  SERIAL.print(", pid: ");
  SERIAL.print(pidValue);
  SERIAL.print(", enabled: ");
  SERIAL.print(enabled ? "yes" : "no");
  SERIAL.print(", simulation: ");
  SERIAL.print(simulationMode ? "yes" : "no");
  SERIAL.print(", should heat: ");
  SERIAL.print(heaterOn ? "yes" : "no");
  SERIAL.print(", relay connected: ");
  SERIAL.print(relay.isConnected() ? "yes" : "no");
  SERIAL.println();
  delay(100);*/
}

void Owen::simulate(float dt) {
  if (relay.isConnected()) {
    simulatedHeaterSpeed += SIMULATION_HEATING_ACCELERATION * dt;
    
    if (simulatedHeaterSpeed > SIMULATION_MAX_HEATING_SPEED) {
       simulatedHeaterSpeed = SIMULATION_MAX_HEATING_SPEED;
    }
    
    simulatedTemperature += simulatedHeaterSpeed * dt;
    
    if (simulatedTemperature > 300) {
      simulatedTemperature = 300;
    }
  } else {
    simulatedHeaterSpeed += SIMULATION_COOLING_ACCELERATION * dt;
    
    if (simulatedHeaterSpeed < SIMULATION_MAX_COOLING_SPEED) {
       simulatedHeaterSpeed = SIMULATION_MAX_COOLING_SPEED;
    }
    
    simulatedTemperature += simulatedHeaterSpeed * dt;
    
    if (simulatedTemperature < 20) {
      simulatedTemperature = 20;
    }
  }
}

float Owen::getTemperature() {
  if (simulationMode) {
    return simulatedTemperature;
  } else {
    return (float)thermocouple.readCelsius(); 
  }
}
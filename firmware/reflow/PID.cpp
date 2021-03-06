#include "PID.h"
#include "Config.h"

#include "../EEPROMex/EEPROMex.h"
#include <Arduino.h>

float PID::getValue(float feedback, float dt) {
  float error = target - feedback;
  float derivative;
  float errorPerSecond;
  float errorDiff;
  
  errorPerSecond = error * dt;
  integral += errorPerSecond;
  errorDiff = error - lastError;
  derivative = errorDiff / dt;
  
  float pValue = error * profile.p;
  float iValue = integral * profile.i;
  float dValue = derivative * profile.d;
  
  /*if (iLimit != -1.0f) {
    float iLimitNegative = -1.0f * iLimit;

    if (iValue < iLimitNegative) {
      iValue = iLimitNegative;
      integral = iLimitNegative / i;
    } else if (iValue > iLimit) {
      iValue = iLimit;
      integral = iLimit / i;
    }
  }*/
  
  float pidValue = pValue + iValue + dValue;

  /*
  //if (counter % 100 == 0) {
    COMM.print("Target: ");
    COMM.print(target);
    COMM.print(", actual: ");
    COMM.print(feedback);
    COMM.print(", error: ");
    COMM.print(error);
    COMM.print(", lastError: ");
    COMM.print(lastError);
    COMM.print(", errorDiff: ");
    COMM.print(errorDiff);
    COMM.print(", p: ");
    COMM.print(pValue);
    COMM.print(", i: ");
    COMM.print(iValue);
    COMM.print(", d: ");
    COMM.print(dValue);
    COMM.print(", output: ");
    COMM.print(pidValue);
    COMM.print(", derivative: ");
    COMM.print(derivative);
    COMM.print(", integral: ");
    COMM.print(integral);
    COMM.print(", dt: ");
    COMM.print(dt);
    COMM.println();
  //}
  */
  
  lastError = error;
  
  counter++;

  return pidValue;
}

void PID::init() {
  initializedAddress = EEPROM.getAddress(sizeof(byte));
  profileAddress = EEPROM.getAddress(sizeof(Profile));
  
  byte isStored = EEPROM.readByte(initializedAddress);
  
  if (isStored != MEMORY_VERSION) {
    initMemory(initializedAddress);
  }
  
  load();
}

void PID::initMemory(int initializedAddress) {
  EEPROM.writeByte(initializedAddress, MEMORY_VERSION);
  
  save();
}

void PID::save() {
  EEPROM.writeBlock(profileAddress, profile);
}

void PID::load() {
  EEPROM.readBlock(profileAddress, profile);
}

void PID::reset() {
  integral = 0.0f;
  lastError = 0.0f;
}

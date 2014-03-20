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
    SERIAL.print("Target: ");
    SERIAL.print(target);
    SERIAL.print(", actual: ");
    SERIAL.print(feedback);
    SERIAL.print(", error: ");
    SERIAL.print(error);
    SERIAL.print(", lastError: ");
    SERIAL.print(lastError);
    SERIAL.print(", errorDiff: ");
    SERIAL.print(errorDiff);
    SERIAL.print(", p: ");
    SERIAL.print(pValue);
    SERIAL.print(", i: ");
    SERIAL.print(iValue);
    SERIAL.print(", d: ");
    SERIAL.print(dValue);
    SERIAL.print(", output: ");
    SERIAL.print(pidValue);
    SERIAL.print(", derivative: ");
    SERIAL.print(derivative);
    SERIAL.print(", integral: ");
    SERIAL.print(integral);
    SERIAL.print(", dt: ");
    SERIAL.print(dt);
    SERIAL.println();
  //}
  */
  
  lastError = error;
  
  counter++;

  return pidValue;
}

void PID::reset() {
  integral = 0.0f;
  lastError = 0.0f;
}

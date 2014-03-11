#include "PID.h"
#include <Arduino.h>

PID::PID(float p, float i, float d, float iLimit) {
  this->p = p;
  this->i = i;
  this->d = d;
  this->iLimit = iLimit;
  this->target = 0.0f;
  this->lastError = 0.0f;
  this->integral = 0.0f;
  this->counter = 0;
}

float PID::getValue(float feedback, float dt) {
  float error = target - feedback;
  float derivative;
  float errorPerSecond;
  float errorDiff;
  
  errorPerSecond = error * dt;
  integral += errorPerSecond;
  errorDiff = error - lastError;
  derivative = errorDiff / dt;
  
  float pValue = error * p;
  float iValue = integral * i;
  float dValue = derivative * d;
  
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

  //if (counter % 100 == 0) {
    Serial.print("Target: ");
    Serial.print(target);
    Serial.print(", actual: ");
    Serial.print(feedback);
    Serial.print(", error: ");
    Serial.print(error);
    /*Serial.print(", lastError: ");
    Serial.print(lastError);
    Serial.print(", errorDiff: ");
    Serial.print(errorDiff);*/
    Serial.print(", p: ");
    Serial.print(pValue);
    Serial.print(", i: ");
    Serial.print(iValue);
    Serial.print(", d: ");
    Serial.print(dValue);
    Serial.print(", output: ");
    Serial.print(pidValue);
    Serial.print(", derivative: ");
    Serial.print(derivative);
    Serial.print(", integral: ");
    Serial.print(integral);
    Serial.print(", dt: ");
    Serial.print(dt);
    Serial.println();
  //}
  
  lastError = error;
  
  counter++;

  return pidValue;
}

void PID::reset() {
  integral = 0.0f;
  lastError = 0.0f;
}

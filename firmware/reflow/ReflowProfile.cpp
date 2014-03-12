#include "ReflowProfile.h"
#include "Config.h"

ReflowProfile::ReflowProfile() {
  // TODO Load from memory 
}

float ReflowProfile::getPreheatTime() {
  return preheatTime * timeMultiplier;
}

float ReflowProfile::getSoakingTime() {
  return soakingTime * timeMultiplier;
}

float ReflowProfile::getReflowTime() {
  return reflowTime * timeMultiplier;
}

float ReflowProfile::getPeakTime() {
  return peakTime * timeMultiplier;
}

float ReflowProfile::getCoolingTime() {
  return coolingTime * timeMultiplier;
}

float ReflowProfile::getTotalTime() {
  return getPreheatTime() + getSoakingTime() + getReflowTime() + getPeakTime() + getCoolingTime(); 
}


float ReflowProfile::getStartTemp() {
  return startTemp;
}

float ReflowProfile::getPreheatTemp() {
  return preheatTemp;
}

float ReflowProfile::getSoakingTemp() {
  return soakingTemp;
}

float ReflowProfile::getReflowTemp() {
  return reflowTemp;
}

float ReflowProfile::getCoolingTemp() {
  return coolingTemp;
}


float ReflowProfile::getTargetTempAt(float seconds) {
  float t1, t2;
  float progress;
  
  if (seconds < getPreheatTime()) {
    t1 = startTemp;
    t2 = preheatTemp;
    progress = seconds / getPreheatTime();
  } else if (seconds < getPreheatTime() + getSoakingTime()) {
    t1 = preheatTemp;
    t2 = soakingTemp;
    progress = (seconds - getPreheatTime()) / getSoakingTime();
  } else if (seconds < getPreheatTime() + getSoakingTime() + getReflowTime()) {
    t1 = soakingTemp;
    t2 = reflowTemp;
    progress = (seconds - getPreheatTime() - getSoakingTime()) / getReflowTime();
  } else if (seconds < getPreheatTime() + getSoakingTime() + getReflowTime() + getPeakTime()) {
    t1 = reflowTemp;
    t2 = reflowTemp;
    progress = (seconds - getPreheatTime() - getSoakingTime() - getReflowTime()) / getPeakTime();
  } else {
    t1 = reflowTemp;
    t2 = coolingTemp;
    progress = (seconds - getPreheatTime() - getSoakingTime() - getReflowTime() - getPeakTime()) / getCoolingTime();
  }

  if (progress > 1.0f) {
    return -1;
  }
  
  return t1 + (float)(t2 - t1) * progress; 
}

float ReflowProfile::getNextTempAt(float seconds) {
  float t1, t2;
  float progress;
  
  if (seconds < getPreheatTime()) {
    return preheatTemp;
  } else if (seconds < getPreheatTime() + getSoakingTime()) {
    return soakingTemp;
  } else if (seconds < getPreheatTime() + getSoakingTime() + getReflowTime()) {
    return reflowTemp;
  } else if (seconds < getPreheatTime() + getSoakingTime() + getReflowTime() + getPeakTime()) {
    return reflowTemp;
  } else {
    return coolingTemp;
  } 
}

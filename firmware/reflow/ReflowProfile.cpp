#include "ReflowProfile.h"
#include "Config.h"

ReflowProfile::ReflowProfile() {
  // TODO Load from memory 
}

float ReflowProfile::getPreheatTime() {
  return preheatTime;
}

float ReflowProfile::getSoakingTime() {
  return soakingTime;
}

float ReflowProfile::getReflowTime() {
  return reflowTime;
}

float ReflowProfile::getPeakTime() {
  return peakTime;
}

float ReflowProfile::getCoolingTime() {
  return coolingTime;
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
  
  if (seconds < preheatTime) {
    t1 = startTemp;
    t2 = preheatTemp;
    progress = (float)seconds / (float)preheatTime;
  } else if (seconds < preheatTime + soakingTime) {
    t1 = preheatTemp;
    t2 = soakingTemp;
    progress = (float)(seconds - preheatTime) / (float)soakingTime;
  } else if (seconds < preheatTime + soakingTime + reflowTime) {
    t1 = soakingTemp;
    t2 = reflowTemp;
    progress = (float)(seconds - preheatTime - soakingTime) / (float)reflowTime;
  } else if (seconds < preheatTime + soakingTime + reflowTime + peakTime) {
    t1 = reflowTemp;
    t2 = reflowTemp;
    progress = (float)(seconds - preheatTime - soakingTime - reflowTime) / (float)peakTime;
  } else {
    t1 = reflowTemp;
    t2 = coolingTemp;
    progress = (float)(seconds - preheatTime - soakingTime - reflowTime - peakTime) / (float)coolingTime;
  }

  if (progress > 1.0f) {
    return -1;
  }
  
  return t1 + (float)(t2 - t1) * progress; 
}

float ReflowProfile::getNextTempAt(float seconds) {
  float t1, t2;
  float progress;
  
  if (seconds < preheatTime) {
    return preheatTemp;
  } else if (seconds < preheatTime + soakingTime) {
    return soakingTemp;
  } else if (seconds < preheatTime + soakingTime + reflowTime) {
    return reflowTemp;
  } else if (seconds < preheatTime + soakingTime + reflowTime + peakTime) {
    return reflowTemp;
  } else {
    return coolingTemp;
  } 
}

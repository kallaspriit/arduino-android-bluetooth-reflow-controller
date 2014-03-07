#include "ReflowProfile.h"
#include "Config.h"

ReflowProfile::ReflowProfile() {
  // TODO Load from memory 
}

int ReflowProfile::getPreheatTime() {
  return preheatTime;
}

int ReflowProfile::getSoakingTime() {
  return soakingTime;
}

int ReflowProfile::getReflowTime() {
  return reflowTime;
}

int ReflowProfile::getPeakTime() {
  return peakTime;
}

int ReflowProfile::getCoolingTime() {
  return coolingTime;
}

int ReflowProfile::getTotalTime() {
  return getPreheatTime() + getSoakingTime() + getReflowTime() + getPeakTime() + getCoolingTime(); 
}


int ReflowProfile::getStartTemp() {
  return startTemp;
}

int ReflowProfile::getPreheatTemp() {
  return preheatTemp;
}

int ReflowProfile::getSoakingTemp() {
  return soakingTemp;
}

int ReflowProfile::getReflowTemp() {
  return reflowTemp;
}

int ReflowProfile::getCoolingTemp() {
  return coolingTemp;
}


int ReflowProfile::getTargetTempAt(int seconds) {
  int t1, t2;
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
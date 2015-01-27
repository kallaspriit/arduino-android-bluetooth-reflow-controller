#ifndef REFLOWPROFILE_H
#define REFLOWPROFILE_H

#include "Config.h"

class ReflowProfile {

public:
  struct Profile {
    // timing
    float timeMultiplier;
    float preheatTime; // TODO use memory
    float soakingTime;
    float reflowTime;
    float peakTime;
    float coolingTime;
    
    // temperatures
    float startTemp;
    float preheatTemp;
    float soakingTemp;
    float reflowTemp;
    float coolingTemp;
  };

  ReflowProfile();
  
  float getTargetTempAt(float seconds);
  float getNextTempAt(float seconds);
  
  float getPreheatTime();
  float getSoakingTime();
  float getReflowTime();
  float getPeakTime();
  float getCoolingTime();
  float getTotalTime();
  
  float getStartTemp();
  float getPreheatTemp();
  float getSoakingTemp();
  float getReflowTemp();
  float getCoolingTemp();
  
  void init();
  void save();
  void load();

private:
  void initMemory(int initializedAddress);

  Profile profile;
  
  int initializedAddress;
  int profileAddress;
  
};

#endif



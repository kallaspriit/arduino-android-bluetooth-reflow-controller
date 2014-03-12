#ifndef REFLOWPROFILE_H
#define REFLOWPROFILE_H

class ReflowProfile {

public:
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

private:
  // temperature profile timing
  float timeMultiplier = 3.0f;
  float preheatTime = 1.5f * 60.0f; // TODO use memory
  float soakingTime = 1.5f * 60.0f;
  float reflowTime = 1.5f * 60.0f;
  float peakTime = 20;
  float coolingTime = 2.0f * 60.0f;
  
  // temperature profile temperatures
  float startTemp = 25.0f; // TODO internal temp
  float preheatTemp = 150.0f;
  float soakingTemp = 170.0f;
  float reflowTemp = 240.0f;
  float coolingTemp = 100.0f;
};

#endif



#ifndef REFLOWPROFILE_H
#define REFLOWPROFILE_H

class ReflowProfile {

public:
  ReflowProfile();
  
  int getTargetTempAt(int seconds);

private:
  // temperature profile timing
  int preheatTime = 1.5f * 60.0f; // TODO use memory
  int soakingTime = 1.5f * 60.0f;
  int reflowTime = 1.0f * 60.0f;
  int peakTime = 20;
  int coolingTime = 2.0f * 60.0f;
  int totalTime = preheatTime + soakingTime + reflowTime + peakTime + coolingTime;
  
  // temperature profile temperatures
  int startTemp = 25; // TODO internal temp
  int preheatTemp = 150;
  int soakingTemp = 170;
  int reflowTemp = 250;
  int coolingTemp = startTemp;
};

#endif



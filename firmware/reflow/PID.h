#ifndef PID_H
#define PID_H

class PID {
  public:
    struct Profile {
      Profile(float p = 1.0f, float i = 0.0f, float d = 0.0f, float iLimit = -1.0f) : p(p), i(i), d(d), iLimit(iLimit) {}
      
      float p;
      float i;
      float d;
      float iLimit;
    };
  
    PID() : profile(), target(0.0f), lastError(0.0f), integral(0.0f) {}
    PID(float p, float i, float d, float iLimit = -1.0f) : profile(p, i, d, iLimit), target(0.0f), lastError(0.0f), integral(0.0f) {}
    void init();
    void setTarget(float target) { this->target = target; }
    float getValue(float feedback, float dt);
    
    void load();
    void save();
    void reset();
    
    Profile profile;

  private:
    void initMemory(int initializedAddress);
  
    float target;
    float lastError;
    float integral;
    unsigned long counter;
    
    int initializedAddress;
    int profileAddress;
};

#endif // PID_H

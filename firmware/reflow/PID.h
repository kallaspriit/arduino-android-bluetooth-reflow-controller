#ifndef PID_H
#define PID_H

class PID {
  public:
    PID() : p(1.0f), i(0.0f), d(0.0f), iLimit(-1.0f), target(0.0f), lastError(0.0f), integral(0.0f) {}
    PID(float p, float i, float d, float iLimit = -1.0f);
    void setTarget(float target) { this->target = target; }
    float getValue(float feedback, float dt);
    void reset();

    float p;
    float i;
    float d;
    float iLimit;

  private:
    float target;
    float lastError;
    float integral;
    unsigned long counter;
};

#endif // PID_H

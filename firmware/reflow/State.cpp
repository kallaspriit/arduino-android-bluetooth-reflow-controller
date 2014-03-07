#include "State.h"
#include "Config.h"

void State::setIntent(int intent) {
  this->intent = intent;
}

int State::popLastIntent() {
  int lastIntent = intent;
  
  intent = INTENT_NONE;
  
  return lastIntent;
}

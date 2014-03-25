#include "State.h"
#include "Config.h"
#include <aJSON.h>

void State::setIntent(int intent) {
  this->intent = intent;
}

int State::popLastIntent() {
  int lastIntent = intent;
  
  intent = INTENT_NONE;
  
  return lastIntent;
}

void State::sendStateInfo() {
  aJsonObject* msg = getStateInfo();
  
  if (msg == NULL) {
    return; 
  }
  
  aJsonStream serialStream(&SERIAL);
  
  aJson.print(msg, &serialStream);
  SERIAL.println();
  aJson.deleteItem(msg); 
}

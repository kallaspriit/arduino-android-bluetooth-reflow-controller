#include "ReflowState.h"
#include "Config.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

ReflowState::ReflowState(Adafruit_PCD8544* display) : display(display) {}

int ReflowState::step(float dt) {
  display->clearDisplay();
  display->drawLine(0, 0, display->width() - 1, display->height() - 1, BLACK);
  display->display();
  
  return popLastIntent();
}

void ReflowState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  setIntent(INTENT_MAIN_MENU);
}


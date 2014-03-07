#include "ReflowState.h"
#include "Config.h"
#include "ReflowProfile.h"
#include "ProfileRenderer.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

ReflowState::ReflowState(Adafruit_PCD8544* display, ReflowProfile* profile) : display(display), profileRenderer(display, profile) {}

int ReflowState::step(float dt) {
  display->clearDisplay();
  
  profileRenderer.render(0, 17, display->width(), display->height() - 17, 300);
  
  display->display();
  
  return popLastIntent();
}

void ReflowState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  setIntent(INTENT_MAIN_MENU);
}


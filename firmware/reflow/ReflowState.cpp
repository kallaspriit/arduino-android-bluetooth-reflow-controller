#include "ReflowState.h"
#include "Config.h"
#include "ReflowProfile.h"
#include "ProfileRenderer.h"
#include "Owen.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

ReflowState::ReflowState(Adafruit_PCD8544* display, Owen* owen, ReflowProfile* profile) :
  display(display),
  owen(owen),
  profileRenderer(display, profile),
  profile(profile),
  reflowDuration(0.0f),
  reflowing(false),
  lastRenderTime(0)
{}

int ReflowState::step(float dt) {
  if (reflowing) {
    reflowDuration += dt; // test for faster progress
  }
  
  if (reflowDuration >= profile->getTotalTime()) {
    reflowing = false;
  }
  
  int sensorTemp = owen->getTemperature();
  int targetTemp = profile->getTargetTempAt(reflowDuration);
  int progressPercentage = reflowDuration * 100 / profile->getTotalTime();
  unsigned long currentTime = millis();
  
  realTemperatures[(int)reflowDuration] = sensorTemp;
  
  owen->setEnabled(reflowing);
  owen->setTargetTemperature(targetTemp);
  owen->step(dt);
  
  // dont render the UI too often
  if (currentTime - lastRenderTime >= 500) {
    display->clearDisplay();
    
    renderTemperatures(sensorTemp, targetTemp);
    profileRenderer.render(0, 19, display->width(), display->height() - 19, reflowDuration, realTemperatures);
    renderProgress(progressPercentage, profile->getTotalTime() - reflowDuration);
    
    display->display();
    
    lastRenderTime = currentTime;
  }

  return popLastIntent();
}

void ReflowState::renderTemperatures(int sensorTemp, int targetTemp) {
  int textX = 0;
  
  if (sensorTemp < 10) {
    textX += 12;
  } else if (sensorTemp < 100) {
    textX += 6;
  }
  
  if (targetTemp < 10) {
    textX += 12;
  } else if (targetTemp < 100) {
    textX += 6;
  }
  
  display->setTextSize(2);
  display->setTextColor(BLACK);
  display->setCursor(textX, 0);
  display->print(sensorTemp);
  display->print("/");
  display->println(targetTemp); 
}

void ReflowState::renderProgress(int percentage, int duration) {
   // draw percentage
  int textX = 20;
  int textY = display->height() - 7;
  int charWidth = 6;
  int charHeight = 7;
  int chars = 7;
  int paddingX = 3;
  int paddingY = 2;
  
  if (percentage < 10) {
    textX += 3;
    chars -= 1;
  } else if (percentage > 99) {
    textX -= 3;
    chars += 1;
  }
  
  int durationValue = duration > 60 ? duration / 60 : duration;
  
  if (durationValue < 10) {
    textX += 3;
    chars -= 1;
  } else if (durationValue > 99) {
    textX -= 3;
    chars += 1;
  }
  
  display->setTextSize(1);
  display->setTextColor(BLACK);
  display->fillRect(textX - paddingX, textY - paddingY, chars * charWidth + paddingX * 2, charHeight + paddingY, WHITE);
  display->setCursor(textX, textY);
  display->print(percentage);
  display->print("% ");
  
  if (duration > 60) {
    display->print(duration / 60);
    display->print("m");
  } else {
    display->print(duration);
    display->print("s");
  }
}

void ReflowState::onEnter() {
  reflowDuration = 0.0f;
  reflowing = true;
  
  // reset simulation
  owen->setSimulationMode(SIMUALATION_MODE);
}

void ReflowState::onExit() {
  owen->setEnabled(false);
  owen->setTargetTemperature(0);
  owen->step(0.1f);
}

void ReflowState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  setIntent(INTENT_MAIN_MENU);
}


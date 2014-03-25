#include "ReflowState.h"
#include "Config.h"
#include "ReflowProfile.h"
#include "Renderer.h"
#include "Owen.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <aJSON.h>

ReflowState::ReflowState(Adafruit_PCD8544* display, Owen* owen, ReflowProfile* profile) :
  display(display),
  owen(owen),
  renderer(display),
  profile(profile),
  reflowDuration(0.0f),
  reflowing(false),
  lastRenderTime(0),
  confirmExitTimeout(0.0f),
  profileInfoIndex(0),
  profileInfoCount(2),
  profileInfoChangeInterval(5.0f),
  profileInfoChangeTimeout(profileInfoChangeInterval)
{}

int ReflowState::step(float dt) {
  float sensorTemp = owen->getTemperature();
  float targetTemp = profile->getTargetTempAt(reflowDuration);
  //float nextTemp = profile->getNextTempAt(reflowDuration);
  int progressPercentage = reflowDuration * 100 / profile->getTotalTime();
  float totalTime = profile->getTotalTime();
  unsigned long currentTime = millis();
  
  if (reflowing) {
    reflowDuration += dt; // test for faster progress
    realTemperatures[(int)reflowDuration] = sensorTemp;
  }
  
  if (reflowDuration >= totalTime) {
    reflowing = false;
    reflowDuration = totalTime;
  }
  
  owen->setEnabled(reflowing);
  //owen->setTargetTemperature(nextTemp);
  owen->setTargetTemperature(targetTemp);
  owen->step(dt);
  
  // update confirmation timer
  if (confirmExitTimeout != 0.0f) {
    confirmExitTimeout -= dt;
    
    if (confirmExitTimeout < 0.0f) {
      confirmExitTimeout = 0.0f;
    }
  }
  
  // update profile info timeout
  profileInfoChangeTimeout -= dt;
  
  if (profileInfoChangeTimeout <= 0.0f) {
    profileInfoIndex = (profileInfoIndex + 1) % profileInfoCount;
    profileInfoChangeTimeout = profileInfoChangeInterval;
  }
  
  // dont render the UI too often
  if (currentTime - lastRenderTime >= 500) {
    display->clearDisplay();
    
    if (confirmExitTimeout != 0.0f) {
      renderConfirmExit(confirmExitTimeout);
    } else {
      renderer.renderProfile(0, 19, display->width(), display->height() - 19, reflowDuration, profile, realTemperatures);
      
      if (!reflowing) {
        renderer.renderTextCentered(0, 0, "Done!", true);
        renderer.renderTextCentered(0, 32, (int)sensorTemp, true, WHITE);
      } else {
        if (profileInfoIndex == 0) {
          renderTemperatures(sensorTemp, targetTemp);
        } else if (profileInfoIndex == 1) {
          renderProgress(progressPercentage, profile->getTotalTime() - reflowDuration);
        }
      }
    }
    
    display->display();
    
    sendStateInfo();
    
    lastRenderTime = currentTime;
  }
  
  stateInfo.duration = reflowDuration;
  stateInfo.reflowing = reflowing;
  stateInfo.currentTemperature = sensorTemp;
  stateInfo.targetTemperature = targetTemp;
  
  return popLastIntent();
}

void ReflowState::renderTemperatures(int sensorTemp, int targetTemp) {
  String text = "";
  
  text = text + sensorTemp + "/" + targetTemp;
  
  renderer.renderTextCentered(0, 0, text, true);
}

void ReflowState::renderProgress(int percentage, int duration) {
  int durationValue = duration > 60 ? duration / 60 : duration;
  String text = "";
  
  text = text + percentage + "% " + durationValue + (duration > 60 ? "m" : "s");
  
  /*if (duration > 60) {
    text += "m";
  } else {
    text += "m";
  }*/
  
  renderer.renderTextCentered(0, 0, text, true);
  
  /*
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
  }*/
}

void ReflowState::renderConfirmExit(float timeout) {
  renderer.renderTextCentered(0, 0, "Cancel?", true);
  renderer.renderTextCentered(0, 18, "Click again");
  renderer.renderTextCentered(0, 27, "to verify");
  renderer.renderTextCentered(0, 40, (int)timeout + 1);
}

void ReflowState::onEnter() {
  confirmExitTimeout = 0.0f;
  reflowDuration = 0.0f;
  reflowing = true;
  
  // reset simulation
  owen->reset();
  owen->setSimulationMode(SIMUALATION_MODE);
}

void ReflowState::onExit() {
  owen->setEnabled(false);
}

void ReflowState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  if (btn == BTN_SELECT) {
    if (reflowing) {
      if (confirmExitTimeout == 0.0f) {
        confirmExitTimeout = 5.0f; // give the user some time to confirm cancelling the reflow process
      } else {
        // user confirmed
        confirmExitTimeout = 0.0f;
        setIntent(INTENT_MAIN_MENU);
      }
    } else {
      setIntent(INTENT_MAIN_MENU);
    }
  } else {
    // cancel exiting process if in progress
    if (confirmExitTimeout != 0.0f) {
      confirmExitTimeout = 0.0f;
    }
  }
}

aJsonObject* ReflowState::getStateInfo() {
  aJsonObject* msg = aJson.createObject();
  aJsonObject* data = aJson.createObject();
  
  /*aJson.addNumberToObject(data, "p", owen->getPID()->profile.p);
  aJson.addNumberToObject(data, "i", owen->getPID()->profile.i);
  aJson.addNumberToObject(data, "d", owen->getPID()->profile.d);*/
  aJson.addNumberToObject(data, "duration", stateInfo.duration);
  aJson.addBooleanToObject(data, "reflowing", stateInfo.reflowing);
  aJson.addNumberToObject(data, "currentTemperature", stateInfo.currentTemperature);
  aJson.addNumberToObject(data, "targetTemperature", stateInfo.targetTemperature);

  aJson.addStringToObject(msg, "type", "reflow-info");
  aJson.addItemToObject(msg, "data", data);

  return msg;
}


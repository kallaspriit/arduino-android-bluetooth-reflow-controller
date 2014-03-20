#include "ConfigurePidState.h"
#include "Config.h"
#include "PID.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

ConfigurePidState::ConfigurePidState(Adafruit_PCD8544* display, PID* pid) : display(display), renderer(display), pid(pid) {}

int ConfigurePidState::step(float dt) {
  display->clearDisplay();
  
  display->setTextColor(BLACK);
  
  String pText = "P: ";
  String iText = "I: ";
  String dText = "D: ";
  
  int valueWidth = 7;
  int valuePrecision = 4;
  char pValue[10], iValue[10], dValue[10];
  
  dtostrf(pid->profile.p, valueWidth, valuePrecision, pValue);
  dtostrf(pid->profile.i, valueWidth, valuePrecision, iValue);
  dtostrf(pid->profile.d, valueWidth, valuePrecision, dValue);
  
  renderer.renderTextRow("Configuration", 0, 0, WHITE);
  renderer.renderTextRow(pText + pValue, 0, 0, BLACK, 11, 13);
  renderer.renderTextRow(iText + iValue, 1, 0, BLACK, 11, 13);
  renderer.renderTextRow(dText + dValue, 2, 0, BLACK, 11, 13);
  
  display->display();
  
  return popLastIntent();
}

void ConfigurePidState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  setIntent(INTENT_MAIN_MENU);
}


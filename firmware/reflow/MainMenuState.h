#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "State.h"

#include "Arduino.h"

class Adafruit_PCD8544;
class Menu;

class MainMenuState : public State {

public:
  MainMenuState(Adafruit_PCD8544* display);
  ~MainMenuState();
  
  void step(unsigned long dt);
  void onKeyPress(int btn, unsigned long duration, boolean repeated);
  /*void onKeyRelease(int btn, unsigned long duration);
  void onKeyDown(int btn);
  void onKeyUp(int btn);*/
  
private:
  Adafruit_PCD8544* display;
  Menu* menu;

};

#endif


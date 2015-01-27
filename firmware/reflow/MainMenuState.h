#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "State.h"
#include "Menu.h"

#include "Arduino.h"

class Adafruit_PCD8544;
class Menu;

class MainMenuState : public State {

public:
  MainMenuState(Adafruit_PCD8544* display);
  
  const char* getName() { return "main-menu"; }
  int step(float dt);
  void onKeyPress(int btn, unsigned long duration, boolean repeated);
  
private:
  Adafruit_PCD8544* display;
  Menu menu;
  static const int itemCount = 6;
  String items[itemCount];

};

#endif



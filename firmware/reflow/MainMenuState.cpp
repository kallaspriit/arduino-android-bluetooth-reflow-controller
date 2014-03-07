#include "MainMenuState.h"
#include "Menu.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

MainMenuState::MainMenuState(Adafruit_PCD8544* display) : display(display), menu(NULL) {
  String items[] = {"Start reflow", "Pick profile", "Show profile", "Learn PID", "About"};
  int itemCount = 5;
  
  menu = new Menu(display, items, itemCount);
}

MainMenuState::~MainMenuState() {
  delete menu;
  menu = NULL; 
}

void MainMenuState::step(unsigned long dt) {
  
}

void MainMenuState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  
}


#include "MainMenuState.h"
#include "Config.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

#define ACTION_START_REFLOW "Start reflow"
#define ACTION_PICK_PROFILE "Pick profile"
#define ACTION_SHOW_PROFILE "Show profile"
#define ACTION_CONFIGURE_PID "PID"
#define ACTION_LEARN "Learn"
#define ACTION_ABOUT "About"

MainMenuState::MainMenuState(Adafruit_PCD8544* display) :
  display(display),
  menu(display)
{
  items[0] = ACTION_START_REFLOW;
  items[1] = ACTION_PICK_PROFILE;
  items[2] = ACTION_SHOW_PROFILE;
  items[3] = ACTION_CONFIGURE_PID;
  items[4] = ACTION_LEARN;
  items[5] = ACTION_ABOUT;
  
  menu.setItems(items, itemCount);
}

int MainMenuState::step(float dt) {
  display->clearDisplay();
  
  menu.render();
  
  display->display();
  
  return popLastIntent();
}

void MainMenuState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  if (btn == BTN_UP) {
      menu.moveUp();
    } else if (btn == BTN_DOWN) {
      menu.moveDown();
    } else if (btn == BTN_SELECT) {
      if (menu.getActive() == ACTION_START_REFLOW) {
        setIntent(INTENT_START_REFLOW);
      } else if (menu.getActive() == ACTION_CONFIGURE_PID) {
        setIntent(INTENT_CONFIGURE_PID);
      }
    }
}


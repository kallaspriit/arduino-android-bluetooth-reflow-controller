#include "MainMenuState.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

MainMenuState::MainMenuState(Adafruit_PCD8544* display) : display(display), items({"Start reflow", "Pick profile", "Show profile", "Learn PID", "About"}), menu(display, items, itemCount) {

}

void MainMenuState::step(unsigned long dt) {
  Serial.print("Step: ");
  Serial.println(dt);
  
  menu.render();
}

void MainMenuState::onKeyPress(int btn, unsigned long duration, boolean repeated) {
  if (btn == 17) { // TODO Config header
      if (menu.activeIndex > 0) {
        menu.activeIndex--;
      }
    } else if (btn == 19) {
      if (menu.activeIndex < menu.itemCount - 1) {
        menu.activeIndex++;
      }
    } else if (btn == 18) {
      
    }
}


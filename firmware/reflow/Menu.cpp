#include "Menu.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

Menu::Menu(Adafruit_PCD8544* display) : display(display), renderer(display), items(NULL), itemCount(0), activeIndex(0) {

}

Menu::Menu(Adafruit_PCD8544* display, String* items, int itemCount) : display(display), renderer(display), items(items), itemCount(itemCount), activeIndex(0) {

}

void Menu::setItems(String* items, int itemCount) {
  this->items = items;
  this->itemCount = itemCount;
}

void Menu::moveUp() {
  if (activeIndex > 0) {
      activeIndex--;
      
      render();
    }
}

void Menu::moveDown() {
  if (activeIndex < itemCount - 1) {
    activeIndex++;
    
    render();
  }
}

String Menu::getActive() {
  return items[activeIndex];
}

void Menu::render() {
  display->setTextSize(1);
  
  int scrollOffset = 0;
  
  if (activeIndex <= 1) {
    scrollOffset = 0;
  } else if (activeIndex == itemCount - 1) {
    scrollOffset = activeIndex - 2;
  } else {
    scrollOffset = activeIndex - 1;
  }
  
  for (int i = 0; i < itemCount; i++) {
    renderer.renderTextRow(items[i], i, scrollOffset, i == activeIndex ? WHITE : BLACK);
  }
}

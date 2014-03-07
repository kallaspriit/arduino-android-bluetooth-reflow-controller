#include "Menu.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

Menu::Menu(Adafruit_PCD8544* display, String* items, int itemCount) : display(display), items(items), itemCount(itemCount) {
  activeIndex = 0;
}

void Menu::render() {
  display->clearDisplay();
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
    renderItem(items[i], i, scrollOffset, i == activeIndex);
  }
  
  display->display();
}

void Menu::renderItem(String text, int index, int scrollOffset, boolean active) {
  int lineHeight = 13;
  int textHeight = 7;
  int padding = (lineHeight - textHeight) / 2;
  int rowX = 0;
  int rowY = lineHeight * index - scrollOffset * lineHeight;
  
  if (active) {
    display->fillRect(0, rowY, display->width() - 1, lineHeight, BLACK);
    display->setTextColor(WHITE);
  } else {
    display->setTextColor(BLACK);
  }
  
  display->setCursor(rowX + 3, rowY + padding);
  display->println(text);
}

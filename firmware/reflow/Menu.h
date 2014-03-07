#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

class Adafruit_PCD8544;

class Menu {

public:
  //Menu(Adafruit_PCD8544* display);
  Menu(Adafruit_PCD8544* display, String* items, int itemCount);
  
  void setItems(String* items, int itemCount);
  void render();
  
  int activeIndex;
  int itemCount;
  
private:
  void renderItem(String text, int index, int scrollOffset, boolean active);

  Adafruit_PCD8544* display;
  String* items;
};

#endif



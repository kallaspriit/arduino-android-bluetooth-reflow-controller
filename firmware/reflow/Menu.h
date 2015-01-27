#ifndef MENU_H
#define MENU_H

#include "Renderer.h"

#include <Arduino.h>

class Adafruit_PCD8544;

class Menu {

public:
  Menu(Adafruit_PCD8544* display);
  Menu(Adafruit_PCD8544* display, String* items, int itemCount);
  
  void setItems(String* items, int itemCount);
  void render();
  
  void moveUp();
  void moveDown();
  
  String getActive();
  
  int activeIndex;
  int itemCount;
  
private:
  Adafruit_PCD8544* display;
  Renderer renderer;
  String* items;
};

#endif



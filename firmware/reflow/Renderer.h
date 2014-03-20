#ifndef RENDERER_H
#define RENDERER_H

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

class ReflowProfile;

class Renderer {

public:
  Renderer(Adafruit_PCD8544* display);
  
  void renderTextCentered(int x, int y, String text, boolean large = false, int color = BLACK, int width = 0);
  void renderProfile(int x, int y, int width, int height, int elapsed, ReflowProfile* profile, int* realTemperatures);
  void renderTextRow(String text, int index, int scrollOffset, int color = BLACK, int lineHeight = 13, int yOffset = 0);
  
private:
  Adafruit_PCD8544* display;

};

#endif



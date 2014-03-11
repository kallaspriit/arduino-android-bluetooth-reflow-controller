#include "Renderer.h"
#include "Config.h"
#include "ReflowProfile.h"

Renderer::Renderer(Adafruit_PCD8544* display, ReflowProfile* profile) : display(display), profile(profile) {}

void Renderer::renderTextCentered(int x, int y, String text, boolean large, int color, int width) {
  if (width <= 0) {
    width = display->width(); 
  }
  
  int charWidth = large ? 12 : 6;
  int textWidth = text.length() * charWidth;
  int drawPos = x + width / 2 - textWidth / 2;
  
  display->setTextSize(large ? 2 : 1);
  display->setTextColor(color);
  
  display->setCursor(drawPos, y);
  display->print(text);
}

void Renderer::renderProfile(int posX, int posY, int windowWidth, int windowHeight, int elapsed, int* realTemperatures) {
  float pixelsPerSecond = (float)windowWidth / (float)profile->getTotalTime();
  float pixelsPerDegree = (float)windowHeight / (float)profile->getReflowTemp();
  
  int startY = posY + windowHeight - 1;
  
  /*display->setTextSize(1);
  display->setTextColor(BLACK);
  display->setCursor(0, posY - 2);
  display->print(profile->getReflowTemp());*/
  
  for (int i = 0; i < profile->getTotalTime(); i++) {
    int profileTemp = profile->getTargetTempAt(i);
    //int realTemp = profileTemp; // TODO get real temp at given time
    int realTemp = realTemperatures[i];
    
    int x = posX + (float)i * pixelsPerSecond;
    int dy = (float)profileTemp * pixelsPerDegree;
    
    if (i > elapsed) {
      display->drawPixel(x, startY - dy, BLACK);
      display->drawPixel(x, startY - dy - 1, BLACK);
      display->drawPixel(x, startY - dy + 1, BLACK);
      display->drawPixel(x + 1, startY - dy, BLACK);
      display->drawPixel(x - 1, startY - dy, BLACK);
    } else {
      dy = (float)realTemp * pixelsPerDegree;
      
      display->drawLine(x, startY, x, startY - dy, BLACK);
      //display->drawPixel(x, startY - dy, WHITE);
    }
  } 
}

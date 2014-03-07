#include "ProfileRenderer.h"
#include "Config.h"
#include "ReflowProfile.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

ProfileRenderer::ProfileRenderer(Adafruit_PCD8544* display, ReflowProfile* profile) : display(display), profile(profile) {}

void ProfileRenderer::render(int posX, int posY, int windowWidth, int windowHeight, int elapsed) {
  float pixelsPerSecond = (float)windowWidth / (float)profile->getTotalTime();
  float pixelsPerDegree = (float)windowHeight / (float)profile->getReflowTemp();
  
  int startY = posY + windowHeight - 1;
  
  for (int i = 0; i < profile->getTotalTime(); i++) {
    int profileTemp = profile->getTargetTempAt(i);
    int realTemp = profileTemp; // TODO get real temp at given time
    
    int x = posX + (float)i * pixelsPerSecond;
    int dy = (float)profileTemp * pixelsPerDegree;
    
    display->drawPixel(x, startY - dy, BLACK); 
    
    if (i < elapsed) {
      dy = (float)realTemp * pixelsPerDegree;
      
      display->drawLine(x, startY, x, startY - dy, BLACK);
    }
  } 
}

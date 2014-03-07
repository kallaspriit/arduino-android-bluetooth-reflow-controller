#ifndef PROFILERENDERER_H
#define PROFILERENDERER_H

class Adafruit_PCD8544;
class ReflowProfile;

class ProfileRenderer {

public:
  ProfileRenderer(Adafruit_PCD8544* display, ReflowProfile* profile);
  
  void render(int x, int y, int width, int height, int elapsed);
  
private:
  Adafruit_PCD8544* display;
  ReflowProfile* profile;

};

#endif



#ifndef STARFIELD_H
#define STARFIELD_H

#include "SpaceThing.h"

class Starfield : public SpaceThing {
protected:
  int _count;
  float _seed;

public:
  Starfield(int count = 21);
  Starfield(float x, float y, int count = 21);
  virtual void draw(Adafruit_ST7735 tft, Viewport view);
  virtual void erase(Adafruit_ST7735 tft, Viewport view);

  void draw_or_erase(Adafruit_ST7735 tft, int cx, int cy, bool erase);
};

#endif

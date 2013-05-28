#ifndef STARFIELD_H
#define STARFIELD_H

#include "SpaceThing.h"

class Starfield : public SpaceThing {
protected:
  int _count;
  float _seed;

public:
  Starfield(float cx, float cy, int count = 21);
  virtual void draw(Adafruit_ST7735 tft, Viewport view);
  virtual void erase(Adafruit_ST7735 tft, Viewport view);
  virtual void step(void);

  virtual void draw_or_erase(Adafruit_ST7735 tft, int scx, int scy, bool erase);
  virtual void draw_pixel(Adafruit_ST7735 tft, int x, int y, int color);
};

#endif

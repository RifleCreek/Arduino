#ifndef PLANET_H
#define PLANET_H

#include "Colors.h"

class Planet : public SpaceThing {
public:
  int _radius, _color;

  Planet(float cx, float cy, int radius, int color)
  : SpaceThing(cx, cy, radius*2, radius*2) {
    _radius = radius;
    _color = color;
  }

  virtual void draw(Adafruit_ST7735 tft, Viewport view) {
    _screen_cx = _cx - view.x1();
    _screen_cy = _cy - view.y1();
    tft.drawCircle(_screen_cx, _screen_cy, _radius, _color);
  }

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_screen_cx, _screen_cy, _radius, BLACK);
  }

};

#endif
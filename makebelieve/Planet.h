#ifndef PLANET_H
#define PLANET_H

#include "Colors.h"

class Planet : public SpaceThing {
public:
  int _radius, _color;

  Planet(float x, float y, int radius, int color) : SpaceThing(x, y) {
    _radius = radius;
    _color = color;
  }

  virtual void draw(Adafruit_ST7735 tft, Viewport view) {
    _screen_x = _x - view.x1();
    _screen_y = _y - view.y1();
    tft.drawCircle(_screen_x, _screen_y, _radius, _color);
  }

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_screen_x, _screen_y, _radius, BLACK);
  }

};

#endif
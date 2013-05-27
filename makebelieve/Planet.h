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

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_x - view.x1(), _y - view.y1(), _radius, BLACK);
  }
  virtual void draw(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_x - view.x1(), _y - view.y1(), _radius, _color);
  }

};

#endif
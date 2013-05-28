#ifndef STARFIELD_MASKED_H
#define STARFIELD_MASKED_H

#include "Starfield.h"
#include "Rectangle.h"
// #include "Geometry.h"

class StarfieldMasked : public Starfield {
public:
  int _mask_radius;
  Rectangle _mask;

  StarfieldMasked(float cx, float cy, int count = 21)
  : Starfield(cx, cy, count) {};

  void set_mask(int mask_cx, int mask_cy, int mask_radius) {
    _mask_radius = mask_radius;
    _mask._x = mask_cx - mask_radius;
    _mask._y = mask_cy - mask_radius;
    _mask._w = mask_radius*2;
    _mask._h = mask_radius*2;
  }

  virtual void draw_pixel(Adafruit_ST7735 tft, int x, int y, int color) {
    if (x > _mask.x1() && x < _mask.x2() && y > _mask.y1() && y < _mask.y2()) {
      // Uncomment to debug starfield mask
      // Starfield::draw_pixel(tft, x, y, RED);
    } else {
      Starfield::draw_pixel(tft, x, y, color);
    }
  }
};

#endif
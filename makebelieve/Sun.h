#ifndef SUN_H
#define SUN_H

#include "Colors.h"
#include "Planet.h"

class Sun : public Planet {
public:
  int _band_count, _band_length;

  Sun(float x, float y, int radius, int color)
  : Planet(x, y, radius, color) {
    _band_count = 13;
    _band_length = 10;    
    _w = (_radius + 4 + _band_length) * 2;
    _h = (_radius + 4 + _band_length) * 2;
  }

  void draw_halo(Adafruit_ST7735 tft, int cx, int cy, int color) {
    for (int i = 0; i < _band_count; i++) {
      float rad = float(i) / _band_count * PI * 2;
      int x1 = cos(rad) * (_radius + 4);
      int y1 = sin(rad) * (_radius + 4);
      int x2 = cos(rad) * (_radius + 4 + _band_length);
      int y2 = sin(rad) * (_radius + 4 + _band_length);
      tft.drawLine(
        cx + x1, cy + y1,
        cx + x2, cy + y2,
        color);
    }    
  }

  virtual void draw(Adafruit_ST7735 tft, Viewport view) {
    _screen_x = _x - view.x1();
    _screen_y = _y - view.y1();
    tft.drawCircle(_screen_x, _screen_y, _radius, _color);
    draw_halo(tft, _screen_x, _screen_y, _color);
  }

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_screen_x, _screen_y, _radius, BLACK);
    draw_halo(tft, _screen_x, _screen_y, BLACK);
  }

};

#endif
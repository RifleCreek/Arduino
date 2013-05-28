#ifndef SUN_H
#define SUN_H

#include "Colors.h"
#include "Planet.h"

class Sun : public Planet {
public:
  int _band_count, _band_length;

  Sun(float cx, float cy, int radius, int color, char* name)
  : Planet(cx, cy, radius, color, name) {
    _band_count = 13;
    _band_length = 10;    
    _w = (_radius + 4 + _band_length) * 2;
    _h = (_radius + 4 + _band_length) * 2;
  }

  void draw_halo(Adafruit_ST7735 tft, int scx, int scy, int color) {
    for (int i = 0; i < _band_count; i++) {
      float rad = float(i) / _band_count * PI * 2;
      int x1 = cos(rad) * (_radius + 4);
      int y1 = sin(rad) * (_radius + 4);
      int x2 = cos(rad) * (_radius + 4 + _band_length);
      int y2 = sin(rad) * (_radius + 4 + _band_length);
      tft.drawLine(
        scx + x1, scy + y1,
        scx + x2, scy + y2,
        color);
    }    
  }

  virtual void draw(Adafruit_ST7735 tft, Viewport view) {
    _screen_cx = _cx - view.x1();
    _screen_cy = _cy - view.y1();
    tft.drawCircle(_screen_cx, _screen_cy, _radius, _color);
    draw_halo(tft, _screen_cx, _screen_cy, _color);
  }

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_screen_cx, _screen_cy, _radius, BLACK);
    draw_halo(tft, _screen_cx, _screen_cy, BLACK);
  }

};

#endif
#ifndef SUN_H
#define SUN_H

#include "Colors.h"
#include "Planet.h"

class Sun : public Planet {
public:

  Sun(float x, float y, int radius, int color)
  : Planet(x, y, radius, color) {}

  void draw_halo(Adafruit_ST7735 tft, int cx, int cy, int color, int bands, int len) {
    for (int i = 0; i < bands; i++) {
      float rad = float(i) / bands * PI * 2;
      int x1 = cos(rad) * (_radius + 4);
      int y1 = sin(rad) * (_radius + 4);
      int x2 = cos(rad) * (_radius + 4 + len);
      int y2 = sin(rad) * (_radius + 4 + len);
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
    draw_halo(tft, _screen_x, _screen_y, _color, 13, 10);
  }

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_screen_x, _screen_y, _radius, BLACK);
    draw_halo(tft, _screen_x, _screen_y, RED, 13, 10);
  }

};

#endif
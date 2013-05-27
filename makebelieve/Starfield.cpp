#include "Starfield.h"
#include "Colors.h"

Starfield::Starfield(int count)
: SpaceThing(), _count(count) {
  _seed = analogRead(0);
  _w = 128;
  _h = 160;
}

Starfield::Starfield(float x, float y, int count)
: SpaceThing(x, y), _count(count) {
  _seed = analogRead(0);
  _w = 128;
  _h = 160;
}


void Starfield::draw(Adafruit_ST7735 tft, Viewport view) {
  _screen_x = _x - view.x1();
  _screen_y = _y - view.y1();
  draw_or_erase(tft, _screen_x, _screen_y, false);
}

void Starfield::erase(Adafruit_ST7735 tft, Viewport view) {
  // if (_screen_x != _erase_screen_x || _screen_y != _erase_screen_y) {
    draw_or_erase(tft, _screen_x, _screen_y, true);
  // }
}

void Starfield::draw_or_erase(Adafruit_ST7735 tft, int cx, int cy, bool erase = false) {
  int i, x, y, color;
  randomSeed(_seed);
  for (i = 0; i < _count; i++) {
    x = cx + random(_w) - _w/2;
    y = cy + random(_h) - _h/2;
    color = (random(2) == 0) ? GRAY : DARK_GRAY;
    if (erase) color = BLACK;
    tft.drawPixel(x, y, color);
  }
}

#include "Starfield.h"
#include "Colors.h"

Starfield::Starfield(int count)
  : SpaceThing(), _count(count) {
  _seed = analogRead(0);
  _w = 128;
  _h = 160;
}

void Starfield::draw(Adafruit_ST7735 tft, Viewport view) {
  draw_or_erase(tft, view, false);
}

void Starfield::erase(Adafruit_ST7735 tft, Viewport view) {
  draw_or_erase(tft, view, true);
}

void Starfield::draw_or_erase(Adafruit_ST7735 tft, Viewport view, bool erase = false) {
  int i, x, y, color;
  randomSeed(_seed);
  for (i = 0; i < _count; i++) {
    x = random(_w) - _w/2 - view.x1();
    y = random(_h) - _h/2 - view.y1();
    color = (random(2) == 0) ? GRAY : DARK_GRAY;
    if (erase) color = BLACK;
    tft.drawPixel(x, y, color);
  }
}

#ifndef PLANET_H
#define PLANET_H

#include <string.h>
#include "Colors.h"

class Planet : public SpaceThing {
public:
  int _radius, _color;
  char _name[20];

  Planet(float cx, float cy, int radius, int color, char* name)
  : SpaceThing(cx, cy, radius*2, radius*2) {
    _radius = radius;
    _color = color;
    strncpy(_name, name, 19); _name[19] = '\0';
  }

  // Use the name of the planet to create a random number seed
  // which we can then use to generate unique planet terrain.
  int seed(void) {
    int value = 0;
    for (uint i = 0; i < sizeof(_name); i++) value += _name[i];
    return value;
  };

  virtual bool can_orbit(void) { return true; }

  virtual void draw(Adafruit_ST7735 tft, Viewport view) {
    _screen_cx = _cx - view.x1();
    _screen_cy = _cy - view.y1();
    tft.drawCircle(_screen_cx, _screen_cy, _radius, _color);
  }

  virtual void erase(Adafruit_ST7735 tft, Viewport view) {
    tft.drawCircle(_screen_cx, _screen_cy, _radius, BLACK);
  }

  virtual void step(SpaceThing* things, int thing_count) {}

  void draw_big_planet(Adafruit_ST7735 tft, float angle, int color) {
    randomSeed(seed());
    int bumps = (int)(TWO_PI*_radius);
    int last_x = 0, last_y = 0;
    for (int i = 0; i <= bumps; i++) {
      int bump_height = random(6) - 3;
      int x = tft.width()/2 + cos(angle + i*(TWO_PI/bumps)) * ((_radius * 10) + bump_height);
      int y = tft.height() + sin(angle + i*(TWO_PI/bumps)) * ((_radius * 10) + bump_height);
      if (last_x != 0 || last_y != 0) {
        tft.drawLine(last_x, last_y, x, y, color);
      }
      last_x = x; last_y = y;
    }
  }
};

#endif
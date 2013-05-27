#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#define SPACE_SHIP_THRUST_MAX 5
#define SPACE_SHIP_ANGULAR_THRUST_MAX 0.5

#include "SpaceThing.h"

class SpaceShip : public SpaceThing {
private:
  void init(float cx, float cy, int size) {
    _cx = cx; _cy = cy; _size = size;
    _x = (int)cx - size; _y = (int)cy - size;
    _w = size*2; _h = size*2;
  }

public:
  int _danger_level, _size;
  float _precise_screen_x, _precise_screen_y;
  float _screen_direction, _screen_thrust;

  SpaceShip() { init(0, 0, 4); };
  SpaceShip(float cx, float cy, int size) { init(cx, cy, size); }

  virtual void erase(Adafruit_ST7735 tft, Viewport view);
  virtual void draw(Adafruit_ST7735 tft, Viewport view);
  virtual void draw_hov(Adafruit_ST7735 tft);
  void draw_in_color(Adafruit_ST7735 tft,
    float cx, float cy, float dir, float thrust,
    int outline_color, int fill_color, int thrust_color);
  void reset(void);
};

#endif

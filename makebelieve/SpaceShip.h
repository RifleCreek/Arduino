#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include "SpaceThing.h"
#include "Planet.h"

class SpaceShip : public SpaceThing {
private:
  void init(float cx, float cy, int size) {
    _cx = cx; _cy = cy; _size = size;
    _x = (int)cx - size; _y = (int)cy - size;
    _w = size*2; _h = size*2;
    _thrust_max = 3.0;
    _angular_thrust_max = 0.1;
    _orbiting_planet = NULL;
  }

public:
  int _danger_level, _size;
  float _thrust_max, _angular_thrust_max;
  float _precise_screen_x, _precise_screen_y;
  float _screen_direction, _screen_thrust;

  float _saved_cx, _saved_cy, _saved_direction, _saved_size;

  bool _orbit_text_needs_erase;
  Planet* _former_orbiting_planet;
  Planet* _orbiting_planet;

  SpaceShip() { init(0, 0, 4); };
  SpaceShip(float cx, float cy, int size) { init(cx, cy, size); }

  virtual void interact(SpaceThing* things[], uint thing_count);
  virtual void erase(Adafruit_ST7735 tft, Viewport view);
  virtual void draw(Adafruit_ST7735 tft, Viewport view);
  virtual void draw_hov(Adafruit_ST7735 tft);
  virtual void erase_hov(Adafruit_ST7735 tft);
  void draw_in_color(Adafruit_ST7735 tft,
    float cx, float cy, float dir, float thrust,
    int outline_color, int fill_color, int thrust_color);
  void draw_hov_in_color(Adafruit_ST7735 tft,
    int thrust_color, int angular_thrust_color);

  void reset(void);
  void save_state(void);
  void restore_state(void);
  void save_size(void);
  void restore_size(void);
};

#endif

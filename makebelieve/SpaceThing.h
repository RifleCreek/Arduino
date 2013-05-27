#ifndef SPACE_THING_H
#define SPACE_THING_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#include "Rectangle.h"
#include "Viewport.h"

class SpaceThing : public Rectangle {
public:
  float _cx, _cy;
  float _direction, _thrust, _angular_thrust;

  int _screen_cx, _screen_cy;

  bool needs_erase;

  SpaceThing(void);
  SpaceThing(float cx, float cy, int w, int h);

  virtual void step(void);
  virtual void erase(Adafruit_ST7735 tft, Viewport view) {};
  virtual void draw(Adafruit_ST7735 tft, Viewport view) {};
  virtual void draw_hov(Adafruit_ST7735 tft) {};

  void set_thrust(float thrust) { _thrust = thrust; }
  void set_angular_thrust(float thrust) { _angular_thrust = thrust; }
};

#endif

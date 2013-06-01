#ifndef PLANET_H
#define PLANET_H

#include <string.h>
#include "Colors.h"

#define BUMP_HEIGHT_MAX 100
#define PAD_MIN_WIDTH 20

enum surface_position {
  surface_pad,
  surface_below,
  surface_above
};

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

  int planetscape_center_x(Adafruit_ST7735 tft) {
    return tft.width()/2;
  };
  
  int planetscape_center_y(Adafruit_ST7735 tft) {
    return tft.height() + _radius*10/2;
  }

  void draw_planetscape(Adafruit_ST7735 tft, float angle, int color) {
    randomSeed(seed());
    int bumps = (int)(TWO_PI*_radius);
    int prev_x = 0, prev_y = 0;
    int center_x = planetscape_center_x(tft);
    int center_y = planetscape_center_y(tft);
    int first_x = 0, first_y = 0;
    for (int i = 0; i <= bumps; i++) {
      int bump_height = random(BUMP_HEIGHT_MAX);
      float theta = i*(TWO_PI/bumps);
      int d = ((_radius * 10) + (bump_height - 30)/10);
      int x = cos(angle + theta) * d + center_x;
      int y = sin(angle + theta) * d + center_y;
      if (i == 0) {
        first_x = x; first_y = y;
      } else if (i == bumps) {
        x = first_x; y = first_y;
      }
      if (prev_x != 0 || prev_y != 0) {
        tft.drawLine(prev_x, prev_y, x, y, color);
      }
      prev_x = x; prev_y = y;
    }
  }

  // JS test box: http://jsbin.com/ixiqiy/15/edit
  void draw_sinusoidal(Adafruit_ST7735 tft, int sx, int sy, int dir) {
    float p1 = (float)random(100) / 100 / 3.5 + 0.05;
    float p2 = (float)random(100) / 100 / 12 + 0.5;
    float p3 = (float)random(100) / 100 / 4 + 0.5;

    float th1 = p1, th2 = p2, th3 = p3;
    for (int i = sx; i > 0 && i < tft.width(); i += dir) {
      int h = sy+(int)(sin(th1)*10 + sin(th2) + sin(th3));
      th1 += p1; th2 += p2; th3 += p3;
      tft.drawFastVLine(i, h, tft.height()-h, _color);
    }
  }

  bool is_below_sinusoidal(int x, int y, int sx, int sy, int max_width, int dir) {
    float p1 = (float)random(100) / 100 / 3.5 + 0.05;
    float p2 = (float)random(100) / 100 / 12 + 0.5;
    float p3 = (float)random(100) / 100 / 4 + 0.5;

    float th1 = p1, th2 = p2, th3 = p3;
    for (int i = sx; i > 0 && i < max_width; i += dir) {
      int h = sy+(int)(sin(th1)*10 + sin(th2) + sin(th3));
      th1 += p1; th2 += p2; th3 += p3;
      if (x == i && y > h) return true;
    }
    return false;
  }

  surface_position get_surface_position(int x, int y, int max_width, int surface_height) {
    randomSeed(seed());

    int pad_w = random(15) + PAD_MIN_WIDTH;
    int pad_x = random(max_width - pad_w) + pad_w/2;
    int pad_y = surface_height;

    // Check landing pad
    if (x >= pad_x && x <= pad_x + pad_w && y > pad_y) return surface_pad;

    // Check sinusoidal left
    randomSeed(seed());
    if (is_below_sinusoidal(x, y, pad_x, pad_y, max_width, -1)) return surface_below;
    // Check sinusoidal right
    randomSeed(seed()+1);
    if (is_below_sinusoidal(x, y, pad_x + pad_w, pad_y, max_width, 1)) return surface_below;

    return surface_above;
  }

  void draw_lander_surface(Adafruit_ST7735 tft, int surface_height) {
    randomSeed(seed());

    int pad_w = random(15) + PAD_MIN_WIDTH;
    int pad_x = random(tft.width() - pad_w) + pad_w/2;
    int pad_y = surface_height;

    tft.fillRect(pad_x, pad_y, pad_w, tft.height() - pad_y, _color);
    tft.drawFastHLine(pad_x, pad_y, pad_w, WHITE);
    // Draw sinusoidal left
    randomSeed(seed());
    draw_sinusoidal(tft, pad_x, pad_y, -1);
    // Draw sinusoidal right
    randomSeed(seed()+1);
    draw_sinusoidal(tft, pad_x + pad_w, pad_y, 1);
  }

  // Recreate the conditions of the random number generator for the big planet,
  // but instead of drawing it, grab the bump height and return.
  int planetscape_bump_height(float angle) {
    randomSeed(seed());
    int bumps = (int)(TWO_PI*_radius);
    for (int i = 0; i <= bumps; i++) {
      int bump_height = random(BUMP_HEIGHT_MAX);
      return bump_height;
    }
  }

};

#endif
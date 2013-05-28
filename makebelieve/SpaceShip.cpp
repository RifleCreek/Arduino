#include "Colors.h"
#include "SpaceShip.h"
#include "Geometry.h"

void SpaceShip::reset() {
  _cx = 0;
  _cy = 0;
  _x = -_w/2;
  _y = -_h/2;
}

void SpaceShip::draw(Adafruit_ST7735 tft, Viewport view) {
  _screen_cx = _cx - view.x1();
  _screen_cy = _cy - view.y1();
  _screen_direction = _direction;
  _screen_thrust = _thrust;
  draw_in_color(tft,
    _screen_cx, _screen_cy, _screen_direction, _screen_thrust,
    WHITE, BLACK, RED);
}

void SpaceShip::erase(Adafruit_ST7735 tft, Viewport view) {
  draw_in_color(tft,
    _screen_cx, _screen_cy, _screen_direction, _screen_thrust,
    BLACK, BLACK, BLACK);
}

void SpaceShip::draw_in_color(Adafruit_ST7735 tft,
float cx, float cy, float dir, float thrust,
int outline_color, int fill_color, int thrust_color) {
  int x1 = cx + cos(dir + PI*2/3)*_size;
  int y1 = cy + sin(dir + PI*2/3)*_size;
  int x2 = cx + cos(dir + PI*4/3)*_size;
  int y2 = cy + sin(dir + PI*4/3)*_size;
  int x3 = cx + cos(dir) * _size*1.5;
  int y3 = cy + sin(dir) * _size*1.5;
  tft.fillTriangle(x1, y1,  x2, y2,  x3, y3,  fill_color);
  tft.drawTriangle(x1, y1,  x2, y2,  x3, y3,  outline_color);
  tft.drawLine(
    cx + cos(dir + PI)*_size,
    cy + sin(dir + PI)*_size,
    cx + cos(dir + PI)*(_size+thrust*2),
    cy + sin(dir + PI)*(_size+thrust*2),
    thrust_color);
}

void SpaceShip::draw_hov(Adafruit_ST7735 tft) {
  draw_hov_in_color(tft, DARK_RED, DARK_BLUE);
}

void SpaceShip::erase_hov(Adafruit_ST7735 tft) {
  tft.drawFastVLine(0, 0, tft.height(), BLACK);
  tft.drawFastHLine(0, 0, tft.width(), BLACK);
}

void SpaceShip::draw_hov_in_color(Adafruit_ST7735 tft, int thrust_color, int angular_thrust_color) {
  int thrust_h, angular_thrust_w;

  // Thrust
  thrust_h = _thrust * tft.height() / _thrust_max;
  tft.drawFastVLine(0, tft.height()-thrust_h, thrust_h, thrust_color);

  // Directional Thrust
  angular_thrust_w = abs((int)(_angular_thrust * tft.width()/2)) /
                     _angular_thrust_max;
  if (_angular_thrust < 0) {
    tft.drawFastHLine(tft.width()/2 - angular_thrust_w, 0, angular_thrust_w, angular_thrust_color);
  } else {
    tft.drawFastHLine(tft.width()/2, 0, angular_thrust_w, angular_thrust_color);
  }
}

/*
void calculate_heat_near(SpaceThing sun) {
  distance_to_sun = distance_to(spaceship_x, spaceship_y, WIDTH, HEIGHT);
  if (distance_to_sun < 100) {
    float red = (1.0 - (distance_to_sun-50)/50);
    if (red > 1.0) red = 1.0;
    spaceship_color = tft.Color565(red*255, 0, 0);
  } else {
    spaceship_color = cBlack;
  }
}
*/

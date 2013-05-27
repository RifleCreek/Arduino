#include "Colors.h"
#include "SpaceShip.h"
#include "Geometry.h"

void SpaceShip::reset() {
  _precise_x = 0;
  _precise_y = 0;
  _x = 0;
  _y = 0;
}

void SpaceShip::draw(Adafruit_ST7735 tft, Viewport view) {
  _precise_screen_x = _precise_x - view.x1();
  _precise_screen_y = _precise_y - view.y1();
  _screen_direction = _direction;
  draw_in_color(tft, _precise_screen_x, _precise_screen_y, _screen_direction,
    WHITE, BLACK, RED);
}

void SpaceShip::erase(Adafruit_ST7735 tft, Viewport view) {
  draw_in_color(tft, _precise_screen_x, _precise_screen_y, _screen_direction,
    BLACK, BLACK, BLACK);
}

void SpaceShip::draw_in_color(Adafruit_ST7735 tft, float cx, float cy, float dir,
int outline_color, int fill_color, int thrust_color) {
  int len = 4;
  int x1 = cx + cos(dir + PI*2/3)*len;
  int y1 = cy + sin(dir + PI*2/3)*len;
  int x2 = cx + cos(dir + PI*4/3)*len;
  int y2 = cy + sin(dir + PI*4/3)*len;
  int x3 = cx + cos(dir) * len*1.5;
  int y3 = cy + sin(dir) * len*1.5;
  tft.fillTriangle(x1, y1,  x2, y2,  x3, y3,  fill_color);
  tft.drawTriangle(x1, y1,  x2, y2,  x3, y3,  outline_color);
  tft.drawLine(
    cx + cos(dir + PI)*len,
    cy + sin(dir + PI)*len,
    cx + cos(dir + PI)*(len+_thrust*2),
    cy + sin(dir + PI)*(len+_thrust*2),
    thrust_color);
}

void SpaceShip::draw_hov(Adafruit_ST7735 tft) {
  int thrust_color = DARK_RED;
  int angular_thrust_color = DARK_BLUE;
  int thrust_h, angular_thrust_w;

  // Thrust
  thrust_h = _thrust * tft.height() / SPACE_SHIP_THRUST_MAX;
  tft.drawFastVLine(0, tft.height()-thrust_h, thrust_h, thrust_color);

  // Directional Thrust
  angular_thrust_w = abs((int)(_angular_thrust * tft.width())) /
                     SPACE_SHIP_ANGULAR_THRUST_MAX;
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

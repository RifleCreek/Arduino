#include "Colors.h"
#include "SpaceShip.h"
#include "Geometry.h"

void SpaceShip::reset() {
  _x = 0;
  _y = 0;
}

void SpaceShip::draw(Adafruit_ST7735 tft, Viewport view) {
  draw_in_color(tft, view, WHITE, BLACK, RED);
}

void SpaceShip::erase(Adafruit_ST7735 tft, Viewport view) {
  draw_in_color(tft, view, BLACK, BLACK, BLACK);
}

void SpaceShip::draw_in_color(Adafruit_ST7735 tft, Viewport view, 
int outline_color, int fill_color, int thrust_color) {
  int len = 4;
  int x1 = _precise_x + cos(_direction + PI*2/3)*len - view.x1();
  int y1 = _precise_y + sin(_direction + PI*2/3)*len - view.y1();
  int x2 = _precise_x + cos(_direction + PI*4/3)*len - view.x1();
  int y2 = _precise_y + sin(_direction + PI*4/3)*len - view.y1();
  int x3 = _precise_x + cos(_direction) * len*1.5 - view.x1();
  int y3 = _precise_y + sin(_direction) * len*1.5 - view.y1();
  tft.fillTriangle(x1, y1,  x2, y2,  x3, y3,  fill_color);
  tft.drawTriangle(x1, y1,  x2, y2,  x3, y3,  outline_color);
  tft.drawLine(
    _precise_x + cos(_direction + PI)*len - view.x1(),
    _precise_y + sin(_direction + PI)*len - view.y1(),
    _precise_x + cos(_direction + PI)*(len+_thrust*2) - view.x1(),
    _precise_y + sin(_direction + PI)*(len+_thrust*2) - view.y1(),
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

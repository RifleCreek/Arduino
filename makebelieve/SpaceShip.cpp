#include <Colors.h>
#include <SpaceShip.h>

void SpaceShip::draw(Adafruit_ST7735 tft, Viewport view) {
  
}

void SpaceShip::erase(Adafruit_ST7735 tft, Viewport view) {
  
}

void SpaceShip::reset() {
  _x = 0;
  _y = 0;
}

void SpaceShip::draw_hov(Adafruit_ST7735 tft) {
  int thrust_color = DARK_RED;
  int angular_thrust_color = DARK_BLUE;
  int thrust_h, angular_thrust_w;

  // Thrust
  thrust_h = _thrust * tft.height() / SPACE_SHIP_THRUST_MULTIPLY;
  tft.drawFastVLine(0, tft.height()-thrust_h, thrust_h, thrust_color);

  // Directional Thrust
  angular_thrust_w = abs((int)(_angular_thrust * tft.width())) /
                     SPACE_SHIP_ANGULAR_THRUST_MULTIPLY;
  if (_angular_thrust < 0) {
    tft.drawFastHLine(tft.width()/2 - angular_thrust_w, 0, angular_thrust_w, angular_thrust_color);
  } else {
    tft.drawFastHLine(tft.width()/2, 0, angular_thrust_w, angular_thrust_color);
  }
}
#include "SpaceThing.h"

SpaceThing::SpaceThing(void)
: Rectangle(0, 0, 5, 5) {
  SpaceThing(0.0, 0.0);
}

SpaceThing::SpaceThing(float x, float y)
: Rectangle((int)x, (int)y, 5, 5), _precise_x(x), _precise_y(y) {
  _direction = 0;
  _thrust = 0;
  _angular_thrust = 0;
}

void SpaceThing::step(void) {
  // Make it easier to "stop"
  if (_thrust < 0.25) _thrust = 0.0;
  // Make it easier to "go straight"
  if (_angular_thrust < 0.025 && _angular_thrust > -0.025) {
    // _angular_thrust = 0.0;
  }
  
  // Move position and direction based on thrust values
  _direction += _angular_thrust;  
  _precise_x += cos(_direction) * _thrust;
  _x = (int)_precise_x;
  _precise_y += sin(_direction) * _thrust;
  _y = (int)_precise_y;
}

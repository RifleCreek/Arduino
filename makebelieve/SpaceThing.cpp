#include "SpaceThing.h"

SpaceThing::SpaceThing(void) {
  Rectangle::init(0.0f, 0.0f, 6, 6);
  SpaceThing::init();
}

SpaceThing::SpaceThing(int x, int y, int w, int h) : _cx(x+w/2), _cy(y+h/2) {
  Rectangle::init(x, y, w, h);
  SpaceThing::init();
}

SpaceThing::SpaceThing(float cx, float cy, int w, int h) : _cx(cx), _cy(cy) {
  Rectangle::init(cx, cy, w, h);
  SpaceThing::init();
}

void SpaceThing::step(void) {
  // Make it easier to "stop"
  if (_thrust < 0.25) _thrust = 0.0;
  // Make it easier to "go straight"
  if (_angular_thrust < 0.025 && _angular_thrust > -0.025) {
    _angular_thrust = 0.0;
  }
  
  // Move position and direction based on thrust values
  _direction += _angular_thrust;  
  _cx += cos(_direction) * _thrust;
  _x = (int)_cx - _w/2;
  _cy += sin(_direction) * _thrust;
  _y = (int)_cy - _h/2;
}

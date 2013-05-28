#include "Viewport.h"
#include "SpaceThing.h"

void Viewport::center_on(SpaceThing& thing) {
  _x = (int)(thing._cx - _w/2);
  _y = (int)(thing._cy - _h/2);
}

void Viewport::center_x_on(SpaceThing& thing) {
  _x = (int)(thing._cx - _w/2);
}

void Viewport::center_y_on(SpaceThing& thing) {
  _y = (int)(thing._cy - _h/2);
}
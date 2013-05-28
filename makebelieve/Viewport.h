#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Rectangle.h"

class SpaceThing;

class Viewport : public Rectangle {
public:
  Viewport(int x, int y, int w, int h) : Rectangle(x, y, w, h) {};

  void center_on(SpaceThing& thing);
  void center_x_on(SpaceThing& thing);
  void center_y_on(SpaceThing& thing);
};

#endif
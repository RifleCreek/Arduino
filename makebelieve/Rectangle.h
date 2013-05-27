#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <Arduino.h>

class Rectangle {
public:
  int _x, _y, _w, _h;
  
  Rectangle(int x, int y, int w, int h) : _x(x), _y(y), _w(w), _h(h) {};

  inline int x1() { return _x; }
  inline int y1() { return _y; }
  inline int x2() { return _x + _w; }
  inline int y2() { return _y + _h; }

  bool overlaps(Rectangle b) {
    return (x1() < b.x2() && x2() > b.x1() &&
            y1() < b.y2() && y2() > b.y1());
  }
};


#endif
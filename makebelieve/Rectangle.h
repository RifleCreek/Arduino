#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <Arduino.h>

class Rectangle {
protected:
  void init(int x, int y, int w, int h) {
    _x = x; _y = y; _w = w; _h = h;
  }

  void init(float cx, float cy, int w, int h) {
    _x = (int)(cx - w/2);
    _y = (int)(cy - h/2);
    _w = w;
    _h = h;
  }

public:
  int _x, _y, _w, _h;
  
  Rectangle(void) {}
  Rectangle(int x, int y, int w, int h) { init(x, y, w, h); }

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
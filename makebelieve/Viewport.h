#ifndef VIEWPORT_H
#define VIEWPORT_H

class Viewport : public Rectangle {

public:
  Viewport(int x, int y, int w, int h) : Rectangle(x, y, w, h) {};
};

#endif
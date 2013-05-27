#ifndef GEOMETRY_H
#define GEOMETRY_H

float distance(float x1, float y1, float x2, float y2) {
  float dx = (x1-x2), dy = (y1-y2);
  return sqrt(dx*dx + dy*dy);
}

#endif
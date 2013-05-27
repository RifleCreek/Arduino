#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

#ifndef HALF_PI
#define HALF_PI 1.5707963267948966192313216916398
#endif

#ifndef TWO_PI
#define TWO_PI 6.283185307179586476925286766559
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.017453292519943295769236907684886
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.295779513082320876798154814105
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.4142135623730950488016887
#endif

float distance(float x1, float y1, float x2, float y2) {
  float dx = (x1-x2), dy = (y1-y2);
  return sqrt(dx*dx + dy*dy);
}

#endif
#ifndef COLORS_H
#define COLORS_H

#define COLOR(r, g, b)  \
  ((((uint8_t)b & 0xF8) << 8) | \
   (((uint8_t)g & 0xFC) << 3) | \
   ((uint8_t)r >> 3))

#define RED       COLOR(255, 0, 0)
#define DARK_RED  COLOR(100, 0, 0)
#define GREEN     COLOR(0, 255, 0)
#define BLUE      COLOR(0, 0, 255)
#define DARK_BLUE COLOR(0, 0, 100)

//bgr
#define MAGENTA   COLOR(255, 0, 255)
#define YELLOW    COLOR(128, 128, 0)
#define CYAN      COLOR(0, 255, 255)
#define DARK_CYAN COLOR(0, 120, 120)
#define ORANGE    COLOR(255, 128, 0)

#define BLACK     COLOR(0, 0, 0)
#define DARK_GRAY COLOR(60, 60, 60)
#define GRAY      COLOR(150, 150, 150)
#define WHITE     COLOR(255, 255, 255)

#endif
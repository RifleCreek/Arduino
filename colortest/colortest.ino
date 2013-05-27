// Define pins used by SPI to communicate with LCD display
#define cs   10
#define dc   9
#define rst  8  // you can also connect this to the Arduino reset

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Define screen size
#define WIDTH 126
#define HEIGHT 158

// Initialize the display object with the pins we chose
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void setup() {
  tft.initR(INITR_REDTAB);   // alternative: INITR_GREENTAB
  tft.fillScreen(ST7735_BLACK);
}

float r = 0, g = 0, b = 0;
int x, y, color;
void loop() {
  for (x = 0; x < WIDTH; x++) {
    for (y = 0; y < WIDTH; y++) {
      r = (float)x / WIDTH * 255;
      g = (float)y / WIDTH * 255;
      color = tft.Color565(b, g, r);
      tft.drawPixel(x, y, color);
    }
  }
}

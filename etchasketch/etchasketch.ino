// Define pins used by potentiometer knobs
#define paddle_l 22
#define paddle_r 21

// Define pins used by buttons
#define button_l 4
#define button_r 5
#define button_m 6

// Define pins used by SPI to communicate with LCD display
#define cs   10
#define dc   9
#define rst  8  // you can also connect this to the Arduino reset

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// Initialize the display object with the pins we chose
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

void setup(void) {
  pinMode(button_l, INPUT);
  pinMode(button_r, INPUT);
  pinMode(button_m, INPUT);
  
  tft.initR(INITR_REDTAB);   // alternative: INITR_GREENTAB
  tft.fillScreen(ST7735_BLACK);
  delay(500);
}

#define PADDLE_MAX 1024
#define WIDTH 126
#define HEIGHT 158

int x, y, color;
int color_cycle = 0;

int get_color(int cycle_value) {
  switch(cycle_value % 5) {
    case 0: return ST7735_BLUE;
    case 1: return ST7735_GREEN;
    case 2: return ST7735_CYAN;
    case 3: return ST7735_MAGENTA;
    case 4: return ST7735_YELLOW;
  }
}

void loop() {
  x = analogRead(paddle_l) * WIDTH / PADDLE_MAX;
  y = analogRead(paddle_r) * HEIGHT / PADDLE_MAX;
  
  if (digitalRead(button_l) == HIGH) {
    color_cycle++;
    delay(400);
  }
  
  color = get_color(color_cycle);
  
  if (digitalRead(button_r) == HIGH) {
    tft.fillScreen(color);
    color_cycle++;
    delay(400);
  } else {
    tft.drawPixel(WIDTH-x, HEIGHT-y, color);
  }

  if (digitalRead(button_m) == HIGH) {
    tft.fillScreen(ST7735_BLACK);
  }
  
  
}


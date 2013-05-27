#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#include "Colors.h"
#include "Controls.h"
#include "SpaceShip.h"
#include "Starfield.h"
#include "Viewport.h"
#include "Planet.h"

// Define pins used by potentiometer knobs
#define PIN_PADDLE_L 22
#define PIN_PADDLE_R 21

// Define pins used by buttons
#define PIN_BUTTON_L     4
#define PIN_BUTTON_R     5
#define PIN_BUTTON_START 6

// Define pins used by SPI to communicate with LCD display
#define PIN_CSEL  10
#define PIN_DATA  9
#define PIN_RESET 8

// Initialize the display object with the pins we chose
Adafruit_ST7735 tft = Adafruit_ST7735(PIN_CSEL, PIN_DATA, PIN_RESET);

// Paddles
PaddleControl paddle_left  = PaddleControl(PIN_PADDLE_L);
PaddleControl paddle_right = PaddleControl(PIN_PADDLE_R);
// Buttons
ButtonControl button_left  = ButtonControl(PIN_BUTTON_L);
ButtonControl button_right = ButtonControl(PIN_BUTTON_R);
ButtonControl button_start = ButtonControl(PIN_BUTTON_START);
// Player's Spaceship
SpaceShip spaceship;
Starfield starfield;
Planet mars(-40, 60, 5, RED),
       earth(40, -55, 5, BLUE),
       venus(-45, 50, 4, GREEN);

Viewport view(
  -ST7735_TFTWIDTH/2, -ST7735_TFTHEIGHT/2,
   ST7735_TFTWIDTH,    ST7735_TFTHEIGHT);

// Main Mode determines which "state" the game is in
#define MODE_TITLE     0
#define MODE_CALIBRATE 1
#define MODE_PLAY      2
#define MODE_BUTTON_UP 3
int main_mode = MODE_TITLE;
int next_mode = MODE_TITLE;

void set_main_mode(int mode) {
  tft.fillScreen(BLACK);
  main_mode = MODE_BUTTON_UP;
  next_mode = mode;
}

// MODE_TITLE (0)
int title_flash = 0;
void mode_title() {
  int mb_x = 6;
  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.setCursor(34, 20);
  tft.println("8 Bit");
  // Typesetting for "Makebelieve"
  tft.setTextColor(BLUE);
  tft.setCursor(mb_x, 40);
  tft.println("Mak");
  tft.setCursor(mb_x+34, 40);
  tft.println("ebe");
  tft.setCursor(mb_x+67, 40);
  tft.println("l");
  tft.setCursor(mb_x+74, 40);
  tft.println("i");
  tft.setCursor(mb_x+83, 40);
  tft.println("eve");
  
  tft.setTextSize(1);
  if (title_flash % 30 < 15)
    tft.setTextColor(GRAY);
  else
    tft.setTextColor(WHITE);
  tft.setCursor(30, 100);
  tft.println("Press Start");
  
  title_flash++;
  
  if (button_start.is_pressed()) {
    set_main_mode(MODE_CALIBRATE);
  }
}
// MODE_CALIBRATE (1)
int cal_x = 0, cal_y = 0, cal_r = 25;
void mode_calibrate() {
  tft.drawFastVLine(cal_x, 0, tft.height(), BLACK);
  tft.drawFastHLine(0, cal_y, tft.width(), BLACK);
  
  cal_x = paddle_right.value(tft.width());
  cal_y = tft.height() - paddle_left.value(tft.height());

  tft.drawCircle(tft.width()/2, tft.height(), cal_r, YELLOW);
  tft.drawFastHLine(tft.width()/2-cal_r+1, tft.height()-1, cal_r*2-1, GRAY);
  tft.drawFastVLine(tft.width()/2, tft.height()-cal_r+1, cal_r, GRAY);

  tft.drawFastVLine(cal_x, 0, tft.height(), RED);
  tft.drawFastHLine(0, cal_y, tft.width(), BLUE);  

  tft.setTextSize(1);
  tft.setTextColor(GRAY);
  tft.setCursor(37, 50);
  tft.println("your ship");
  tft.setTextColor(WHITE);
  tft.setCursor(37, 62);
  tft.println("IS READY!");

  tft.setCursor(37, 107);
  tft.println("start the");
  tft.setCursor(46, 119);
  tft.println("engine");

  if (button_start.is_pressed() ||
      (cal_y == tft.height()-1 && cal_x >= tft.width()/2-3 && cal_x <= tft.width()/2+3) ) {
    spaceship.reset();
    set_main_mode(MODE_PLAY);
  }
   
  delay(30);
}

// MODE_PLAY (2)
void mode_play() {
  starfield.erase(tft, view);
  spaceship.erase(tft, view);

  mars.erase(tft, view);
  earth.erase(tft, view);
  venus.erase(tft, view);

  spaceship.set_thrust(
    paddle_left.value(SPACE_SHIP_THRUST_MAX));
  spaceship.set_angular_thrust(
    paddle_right.value(
      -SPACE_SHIP_ANGULAR_THRUST_MAX,
       SPACE_SHIP_ANGULAR_THRUST_MAX));
  // spaceship.set_angular_thrust(1.0);
    // paddle_right.value(SPACE_SHIP_THRUST_MAX));
  
  spaceship.step();

  starfield.draw(tft, view);

  mars.draw(tft, view);
  earth.draw(tft, view);
  venus.draw(tft, view);

  spaceship.draw(tft, view);

  delay(30);

  if (button_start.is_pressed()) {
    set_main_mode(MODE_CALIBRATE);
  }
}

// MODE_BUTTON_UP (3)
void mode_button_up() {
  if (!button_start.is_pressed()) {
    main_mode = next_mode;
  }
}

void setup() {
  tft.initR();
  tft.fillScreen(BLACK);
  Serial.begin(9600);

  Serial.print("view ");
  Serial.print(view.x1());
  Serial.print(", ");
  Serial.print(view.y1());
  Serial.print("\n");
}

void loop() {
  switch(main_mode) {
    case MODE_TITLE:     mode_title();     break;
    case MODE_CALIBRATE: mode_calibrate(); break;
    case MODE_PLAY:      mode_play();      break;
    case MODE_BUTTON_UP: mode_button_up(); break;
  }
}
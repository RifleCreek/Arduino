#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#include "Colors.h"
#include "Controls.h"
#include "SpaceShip.h"
#include "Starfield.h"
#include "Viewport.h"
#include "Planet.h"
#include "Sun.h"

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

Starfield sf00(-ST7735_TFTWIDTH, -ST7735_TFTHEIGHT);
Starfield sf01(               0, -ST7735_TFTHEIGHT);
Starfield sf02(ST7735_TFTWIDTH,  -ST7735_TFTHEIGHT);
Starfield sf10(-ST7735_TFTWIDTH, 0);
Starfield sf11(               0, 0);
Starfield sf12(ST7735_TFTWIDTH,  0);
Starfield sf20(-ST7735_TFTWIDTH, ST7735_TFTHEIGHT);
Starfield sf21(               0, ST7735_TFTHEIGHT);
Starfield sf22(ST7735_TFTWIDTH,  ST7735_TFTHEIGHT);

Planet mars(-45, -50, 5, RED, (char*)"Mars");
Planet earth(40, -55, 5, BLUE, (char*)"Earth");
Planet venus(-40, 60, 4, GREEN, (char*)"Venus");
Planet mercury(95, 90, 3, ORANGE, (char*)"Mercury");
Planet jupiter(195, -100, 10, ORANGE, (char*)"Jupiter");
Planet uranus(10, 200, 7, DARK_CYAN, (char*)"Uranus");
Planet neptune(235, 120, 9, GRAY, (char*)"Neptune");
Sun sun(50, 60, 19, YELLOW, (char*)"Sun");

// Player's Spaceship
SpaceShip spaceship;

SpaceThing* things[] = {
  &sf00,
  &sf01,
  &sf02,
  &sf10,
  &sf11,
  &sf12,
  &sf20,
  &sf21,
  &sf22,
  &mars,
  &earth,
  &venus,
  &mercury,
  &jupiter,
  &uranus,
  &neptune,
  &sun,
  &spaceship
};
uint space_thing_count = sizeof(things) / sizeof(SpaceThing*);

Viewport view(
  -ST7735_TFTWIDTH/2, -ST7735_TFTHEIGHT/2,
   ST7735_TFTWIDTH,    ST7735_TFTHEIGHT);

Viewport lander_view(
  -ST7735_TFTWIDTH/2, -ST7735_TFTHEIGHT/2,
   ST7735_TFTWIDTH,    ST7735_TFTHEIGHT);

// Main Mode determines which "state" the game is in
#define MODE_TITLE     0
#define MODE_CALIBRATE 1
#define MODE_SPACE     2
#define MODE_LANDER    3
#define MODE_BUTTON_UP 999
int main_mode = MODE_TITLE;
int next_mode = MODE_TITLE;
bool space_explorer_mode = false;

void set_main_mode(int mode) {
  tft.fillScreen(BLACK);
  main_mode = MODE_BUTTON_UP;
  next_mode = mode;
}

void player_control(SpaceShip& spaceship,
PaddleControl& pad_thrust,
PaddleControl& pad_turn) {
  spaceship.set_thrust(
    pad_thrust.value(spaceship._thrust_max));
  spaceship.set_angular_thrust(
    pad_turn.value(
      -spaceship._angular_thrust_max,
       spaceship._angular_thrust_max));
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
  
  cal_x = paddle_right.value(tft.width()-1);
  cal_y = tft.height() - paddle_left.value(tft.height()-1);

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

  // if (button_start.is_pressed() ||
  if ((cal_y == tft.height()-1 && cal_x >= tft.width()/2-3 && cal_x <= tft.width()/2+3) ) {
    spaceship.reset();
    set_main_mode(MODE_SPACE);
  }
   
  delay(30);
}

// MODE_SPACE (2)
void mode_space() {
  // EVENT LOOP
  for (uint i = 0; i < space_thing_count; i++) {
    if(things[i]->_needs_erase) {
      things[i]->erase(tft, view);
    }
    things[i]->interact(things, space_thing_count);
    things[i]->step();
    if (view.overlaps(*things[i])) {
      things[i]->draw(tft, view);
      things[i]->_needs_erase = true;
    }
  }

  // HOV
  for (uint i = 0; i < space_thing_count; i++) {
    things[i]->erase_hov(tft);
    things[i]->draw_hov(tft);
  }

  // CONTROL
  player_control(spaceship, paddle_left, paddle_right);
  
  // Explorer mode
  if (space_explorer_mode) view.center_on(spaceship);

  // WAIT
  delay(30);

  if (button_right.is_pressed() && spaceship._orbiting_planet != NULL) {
    set_main_mode(MODE_LANDER);
  }

  if (button_start.is_pressed()) {
    if (space_explorer_mode) {
      space_explorer_mode = false;
      set_main_mode(MODE_CALIBRATE);
    } else {
      space_explorer_mode = true;
      set_main_mode(MODE_SPACE);
    }
  }
}

// MODE_LANDER (3)
float planet_angle = 0.0;
bool mode_lander_init = true;
float prev_player_cx, prev_player_cy, prev_player_dir;
void mode_lander() {
  if (mode_lander_init) {
    mode_lander_init = false;
    prev_player_cx = spaceship._cx;
    prev_player_cy = spaceship._cy;
    prev_player_dir = spaceship._direction;
    spaceship._cx = 0;
    spaceship._cy = -20;
    spaceship._direction = 0;
    lander_view.center_x_on(spaceship);
  }
  tft.setTextSize(1);
  tft.setTextColor(GRAY);
  tft.setCursor(64-strlen(spaceship._orbiting_planet->_name)*3, 40);
  tft.println(spaceship._orbiting_planet->_name);

  spaceship.erase(tft, lander_view);
  spaceship.step();
  spaceship.draw(tft, lander_view);

  spaceship._orbiting_planet->draw_big_planet(tft, planet_angle, BLACK);
  planet_angle = -spaceship._cx / TWO_PI / 10;
  spaceship._orbiting_planet->draw_big_planet(tft, planet_angle, spaceship._orbiting_planet->_color);

  // CONTROL
  player_control(spaceship, paddle_left, paddle_right);
  lander_view.center_x_on(spaceship);

  delay(30);

  if (spaceship._cy < -tft.height()/2) { // button_right.is_pressed()
    mode_lander_init = true;
    spaceship._cx = prev_player_cx;
    spaceship._cy = prev_player_cy;
    spaceship._direction = prev_player_dir;
    set_main_mode(MODE_SPACE);
  }
}

// MODE_BUTTON_UP (999)
void mode_button_up() {
  if (!button_start.is_pressed() && !button_right.is_pressed()) {
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
    case MODE_SPACE:     mode_space();     break;
    case MODE_LANDER:    mode_lander();    break;
    case MODE_BUTTON_UP: mode_button_up(); break;
  }
}

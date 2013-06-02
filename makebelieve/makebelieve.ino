#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#include "Colors.h"
#include "Controls.h"
#include "SpaceShip.h"
#include "Starfield.h"
#include "StarfieldMasked.h"
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

Starfield sf00(-ST7735_TFTWIDTH, -ST7735_TFTHEIGHT, 13);
Starfield sf01(               0, -ST7735_TFTHEIGHT, 21);
Starfield sf02(ST7735_TFTWIDTH,  -ST7735_TFTHEIGHT, 13);
Starfield sf10(-ST7735_TFTWIDTH, 0, 21);
Starfield sf11(               0, 0, 21);
Starfield sf12(ST7735_TFTWIDTH,  0, 21);
Starfield sf20(-ST7735_TFTWIDTH, ST7735_TFTHEIGHT, 13);
Starfield sf21(               0, ST7735_TFTHEIGHT, 21);
Starfield sf22(ST7735_TFTWIDTH,  ST7735_TFTHEIGHT, 13);

Starfield sf31(ST7735_TFTWIDTH*2, 0, 13);
Starfield sf41(ST7735_TFTWIDTH*3, 0, 13);
Starfield sf51(ST7735_TFTWIDTH*4, 0, 13);

Starfield sf60(ST7735_TFTWIDTH*5, ST7735_TFTHEIGHT*(-1), 13);
Starfield sf61(ST7735_TFTWIDTH*5, ST7735_TFTHEIGHT*0, 21);
Starfield sf62(ST7735_TFTWIDTH*5, ST7735_TFTHEIGHT*1, 13);
Starfield sf70(ST7735_TFTWIDTH*6, ST7735_TFTHEIGHT*(-1), 21);
Starfield sf71(ST7735_TFTWIDTH*6, ST7735_TFTHEIGHT*0, 21);
Starfield sf72(ST7735_TFTWIDTH*6, ST7735_TFTHEIGHT*1, 21);
Starfield sf80(ST7735_TFTWIDTH*7, ST7735_TFTHEIGHT*(-1), 13);
Starfield sf81(ST7735_TFTWIDTH*7, ST7735_TFTHEIGHT*0, 21);
Starfield sf82(ST7735_TFTWIDTH*7, ST7735_TFTHEIGHT*1, 13);

StarfieldMasked sf_planetscape(0, 0, 80);

Planet mars(ST7735_TFTWIDTH*6-45, -50, 5, RED, (char*)"Mars");
Planet earth(ST7735_TFTWIDTH*6+40, -55, 5, BLUE, (char*)"Earth");
Planet venus(ST7735_TFTWIDTH*6-40, 60, 4, GREEN, (char*)"Venus");
Planet mercury(ST7735_TFTWIDTH*6+95, 90, 3, ORANGE, (char*)"Mercury");
Planet jupiter(ST7735_TFTWIDTH*6+195, -100, 10, ORANGE, (char*)"Jupiter");
Planet uranus(ST7735_TFTWIDTH*6+10, 200, 7, DARK_CYAN, (char*)"Uranus");
Planet neptune(ST7735_TFTWIDTH*6+235, 120, 9, GRAY, (char*)"Neptune");
Sun sun(ST7735_TFTWIDTH*6+50, 60, 19, YELLOW, (char*)"Sol");

Planet canon(-35, -20, 4, GRAY, (char*)"Canon");
Sun canon_sun(50, 60, 15, ORANGE, (char*)"XN210C");

// Reference to "current planet" if in planetscape or lander modes
Planet* planet;

// Player's Spaceship
SpaceShip spaceship;

SpaceThing* things[] = {

  &sf00, &sf01, &sf02,
  &sf10, &sf11, &sf12,
  &sf20, &sf21, &sf22,
  &sf31,
  &sf41,
  &sf51,
  &sf60, &sf61, &sf62,
  &sf70, &sf71, &sf72,
  &sf80, &sf81, &sf82,
  &canon,
  &canon_sun,
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

Viewport planetscape_view(
  -ST7735_TFTWIDTH/2, -ST7735_TFTHEIGHT/2,
   ST7735_TFTWIDTH,    ST7735_TFTHEIGHT);

Viewport zero_view(
  -ST7735_TFTWIDTH/2, -ST7735_TFTHEIGHT/2,
   ST7735_TFTWIDTH,    ST7735_TFTHEIGHT);

int shape_down_arrow[] = {
 -2, -2,
  2, -2,
  2,  2,
  3,  2,
  0,  4,
 -3,  2,
 -2,  2,
 -2, -2
};
int shape_down_arrow_size = 8;

// Main Mode determines which "state" the game is in
#define MODE_TITLE       0
#define MODE_CALIBRATE   1
#define MODE_SPACE       2
#define MODE_PLANETSCAPE 3
#define MODE_LANDER      4
#define MODE_SURFACE     5
#define MODE_STORY       100
#define MODE_WAIT_BUTTON 998
#define MODE_BUTTON_UP   999
int main_mode = MODE_TITLE;
int next_mode = MODE_TITLE;
bool space_explorer_mode = false;
bool button_up_clear_screen = true;
bool never_landed_on_planet_before = true;

#define STORY_INTRO  0
#define STORY_INTRO2 1
#define STORY_INTRO3 2
int story_sequence = 0;

void print(char* text, int color=WHITE) {
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void centered_text(const char text[], int color=GRAY, int y=40) {
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.setCursor(64-strlen(text)*3, y);
  tft.println((char*)text);
}

void draw_shape(int x, int y, int path[], int count, int color, int m=1) {
  for (int i = 0; i < count-1; i++) {
    tft.drawLine(x+m*path[i*2], y+m*path[i*2+1], x+m*path[i*2+2], y+m*path[i*2+3], color);
  }
}

void set_main_mode(int mode) {
  button_up_clear_screen = true;
  main_mode = MODE_BUTTON_UP;
  next_mode = mode;
  if (next_mode == MODE_TITLE) {
    space_explorer_mode = false;
    story_sequence = STORY_INTRO;
    never_landed_on_planet_before = true;

    view._x = -ST7735_TFTWIDTH/2;
    view._y = -ST7735_TFTHEIGHT/2;
  }
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
  
  if (title_flash % 30 < 15)
    centered_text("Press Start", GRAY, 100);
  else
    centered_text("Press Start", WHITE, 100);

  centered_text("(R skips story)", GRAY, 112);
  
  title_flash++;
  
  if (button_start.is_pressed()) {
    set_main_mode(MODE_STORY);
  }
  if (button_right.is_pressed()) {
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
  if (cal_y > tft.height()-1) cal_y = tft.height()-1;

  tft.drawCircle(tft.width()/2, tft.height()-3, cal_r, YELLOW);
  tft.drawFastHLine(tft.width()/2-cal_r+1, tft.height()-3, cal_r*2-1, GRAY);
  tft.drawFastVLine(tft.width()/2, tft.height()-cal_r+1-3, cal_r+3, GRAY);

  tft.drawFastVLine(cal_x, 0, tft.height(), RED);
  tft.drawFastHLine(0, cal_y, tft.width(), BLUE);  

  centered_text("your ship", GRAY, 20);
  centered_text("IS READY!", WHITE, 32);

  centered_text("start the", WHITE, 87);
  centered_text("engine", WHITE, 99);

  draw_shape(tft.width()/2, 115, shape_down_arrow, shape_down_arrow_size, YELLOW, 2);

  // if (button_start.is_pressed() ||
  if ((cal_y >= tft.height()-3 && cal_x >= tft.width()/2-3 && cal_x <= tft.width()/2+3) ) {
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
  if (never_landed_on_planet_before) {
    if (spaceship._former_orbiting_planet != NULL && spaceship._orbiting_planet == NULL) {
      centered_text("(press R", BLACK, 48);
      centered_text("to land)", BLACK, 56);
    }
    if (spaceship._orbiting_planet != NULL) {
      centered_text("(press R", GRAY, 48);
      centered_text("to land)", GRAY, 56);
    }
  }

  // CONTROL
  player_control(spaceship, paddle_left, paddle_right);
  
  // Explorer mode
  if (space_explorer_mode) view.center_on(spaceship);

  // Lost in space?
  if (spaceship._cx < view.x1() - tft.width()  || spaceship._cx > view.x2() + tft.width() ||
      spaceship._cy < view.y1() - tft.height() || spaceship._cy > view.y2() + tft.height()) {
      centered_text("*lost in space*", GRAY);
      delay(800);
      set_main_mode(MODE_TITLE);
  }

  // WAIT
  delay(30);

  if (button_right.is_pressed() && spaceship._orbiting_planet != NULL) {
    mode_planetscape_init_for_descent();
    set_main_mode(MODE_PLANETSCAPE);
  }

  if (button_left.is_pressed()) {
    if (space_explorer_mode) {
      space_explorer_mode = false;
      set_main_mode(MODE_TITLE);
    } else {
      space_explorer_mode = true;
      set_main_mode(MODE_SPACE);
    }
  }
}

int planetscape_planet_surface = 0;
void mode_planetscape_init_for_descent() {
  never_landed_on_planet_before = false;
  planet = spaceship._orbiting_planet;
  sf_planetscape.set_mask(
    planet->planetscape_center_x(tft),
    planet->planetscape_center_y(tft),
    planet->_radius * 10 + 3);
  planetscape_planet_surface = planet->planetscape_center_y(tft) -
    (planet->_radius * 10) - 12 -
    tft.height()/2;
  spaceship.save_state();
  spaceship._cx = 0;
  spaceship._cy = -20;
  planetscape_view.center_x_on(spaceship);  
}

void mode_planetscape_init_for_ascent() {
  if (planet == NULL) set_main_mode(MODE_TITLE);
  spaceship.restore_size();
  spaceship._cx = 0;
  spaceship._cy = planetscape_planet_surface - 5;
}

// MODE_PLANETSCAPE (3)
float planet_angle = 0.0;
void mode_planetscape() {
  tft.setTextSize(1);
  tft.setTextColor(GRAY);
  tft.setCursor(64-strlen(planet->_name)*3, 40);
  tft.println(planet->_name);

  spaceship.erase(tft, planetscape_view);
  spaceship.step();
  sf_planetscape.draw(tft, zero_view);
  spaceship.draw(tft, planetscape_view);

  planet->draw_planetscape(tft, planet_angle, BLACK);
  planet_angle = -spaceship._cx / TWO_PI / 10;
  planet->draw_planetscape(tft, planet_angle, planet->_color);

  // CONTROL
  player_control(spaceship, paddle_left, paddle_right);
  planetscape_view.center_x_on(spaceship);

  delay(30);

  if (spaceship._cy < -tft.height()/2) { // leaves "upward"
    spaceship.restore_state();
    set_main_mode(MODE_SPACE);
  } else if (spaceship._cy >= planetscape_planet_surface) {
    mode_lander_init_for_descent();
    set_main_mode(MODE_LANDER);
  }
}

float lander_gravity = 0;
int lander_planet_surface;
bool mode_lander_init_bg = true;
void mode_lander_init_for_descent() {
  if (planet == NULL) set_main_mode(MODE_TITLE);

  lander_planet_surface = tft.height() - 15;
  spaceship._cx = 0;
  spaceship._cy = -40;

  spaceship.save_size();
  spaceship._size = 6;

  mode_lander_init_bg = true;
  lander_gravity = (planet->_radius / 10.0);
}

void mode_lander_init_for_ascent() {
  mode_lander_init_bg = true;

  spaceship._cy -= 10;
  spaceship._direction = -PI/2;
}

void mode_lander(void) {
  if (mode_lander_init_bg) {
    planet->draw_lander_surface(tft, 80);
    mode_lander_init_bg = false;
  } else {
    spaceship.erase(tft, zero_view);
  }
  spaceship.step();
  spaceship._cy += lander_gravity;
  spaceship.draw(tft, zero_view);

  switch(planet->get_surface_position(
  spaceship._cx + tft.width()/2,
  spaceship._cy + tft.height()/2 + spaceship._size,
  tft.width(), 80)) {
    case surface_above: break;
    case surface_pad:
      tft.setTextSize(1);
      tft.setTextColor(GRAY);
      tft.setCursor(64-strlen("Landed!")*3, 40);
      tft.println("Landed!");
      delay(800);
      set_main_mode(MODE_SURFACE);
      break;
    case surface_below:
      tft.setTextSize(1);
      tft.setTextColor(GRAY);
      tft.setCursor(64-strlen("*crash*")*3, 40);
      tft.println("*crash*");
      delay(800);
      set_main_mode(MODE_TITLE);
  }

  // CONTROL
  player_control(spaceship, paddle_left, paddle_right);

  delay(30);

  if (spaceship._cy < -tft.height()/2) { // leaves "upward"
    mode_planetscape_init_for_ascent();
    set_main_mode(MODE_PLANETSCAPE);
  }
}

void mode_surface() {
  tft.setTextSize(1);
  tft.setTextColor(GRAY);
  tft.setCursor(64-strlen("SURFACE")*3, 40);
  tft.println("SURFACE");
  delay(30);

  if (button_right.is_pressed()) {
    mode_lander_init_for_ascent();
    set_main_mode(MODE_LANDER);
  }
}

void mode_story() {
  switch(story_sequence) {
    case STORY_INTRO:
      print((char*)"\n\n"
                   "When Rachel awoke,\n"
                   "her parents did not\n"
                   "wake with her. They\n"
                   "were in cryosleep,\n"
                   "but nothing could\n"
                   "rouse them.\n\n");
      story_sequence++;
      break;
    case STORY_INTRO2:
      print((char*)"\n\n"
                   "She was the daughter\n"
                   "of two renowned\n"
                   "Xenoarchivists who\n"
                   "had for the past\n"
                   "decade tried to\n"
                   "unlock the mysteries\n"
                   "of an ancient\n"
                   "planetary archive.");
      story_sequence++;
      break;
    case STORY_INTRO3:
      print((char*)"\n\n"
                   "Now she was alone--\n"
                   "in a sector of the\n"
                   "galaxy frequented by\n"
                   "no one.\n\n\n"
                   "    What could a\n"
                   "  12 year old girl\n"
                   "    possibly do?");
      next_mode = MODE_CALIBRATE;
      story_sequence++;
      break;

  }
  main_mode = MODE_WAIT_BUTTON;
  delay(30);
}

// MODE_WAIT_BUTTON (998)
void mode_wait_button() {
  if (button_start.is_pressed() ||
      button_right.is_pressed() ||
      button_left.is_pressed()) {
    button_up_clear_screen = true;
    main_mode = MODE_BUTTON_UP;
  }
  delay(30);
}

// MODE_BUTTON_UP (999)
void mode_button_up() {
  if (!button_start.is_pressed() && 
      !button_right.is_pressed() &&
      !button_left.is_pressed()) {
    main_mode = next_mode;
    if (button_up_clear_screen) {
      tft.fillScreen(BLACK);
    }
  }
}


void setup() {
  tft.initR();
  tft.fillScreen(BLACK);
  Serial.begin(9600);
}

void loop() {
  switch(main_mode) {
    case MODE_TITLE:       mode_title();       break;
    case MODE_CALIBRATE:   mode_calibrate();   break;
    case MODE_SPACE:       mode_space();       break;
    case MODE_PLANETSCAPE: mode_planetscape(); break;
    case MODE_LANDER:      mode_lander();      break;
    case MODE_SURFACE:     mode_surface();     break;
    case MODE_STORY:       mode_story();       break;
    case MODE_WAIT_BUTTON: mode_wait_button(); break;
    case MODE_BUTTON_UP:   mode_button_up();   break;
  }
  // if (button_start.is_pressed() &&
  //     main_mode != MODE_TITLE && 
  //     main_mode != MODE_WAIT_BUTTON &&
  //     main_mode != MODE_BUTTON_UP &&
  //     main_mode != MODE_STORY) {
  //   set_main_mode(MODE_TITLE);
  // }
}

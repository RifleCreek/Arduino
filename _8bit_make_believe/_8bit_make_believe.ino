#define ANALOG_MAX 1024

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

// Define screen size
#define WIDTH 126
#define HEIGHT 158

#define THRUST_MULTIPLY 5
#define DIR_THRUST_MULTIPLY 0.5

// Initialize the display object with the pins we chose
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

int cRed      = tft.Color565(255, 0, 0);
int cDarkRed  = tft.Color565(100, 0, 0);
int cGreen    = tft.Color565(0, 255, 0);
int cBlue     = tft.Color565(0, 0, 255);
int cDarkBlue = tft.Color565(0, 0, 100);

//bgr
int cMagenta  = tft.Color565(255, 0, 255);
int cYellow   = tft.Color565(128, 128, 0);
int cCyan     = tft.Color565(0, 255, 255);
int cOrange   = tft.Color565(255, 128, 0);

int cBlack    = tft.Color565(0, 0, 0);
int cDarkGray = tft.Color565(60, 60, 60);
int cGray     = tft.Color565(150, 150, 150);
int cWhite    = tft.Color565(255, 255, 255);

long starfield_seed;
// Main Mode determines which "state" the game is in
// 0: title screen
// 1: calibrate
// 2: play
int main_mode = 0;
int bg_color = cBlack;

float pi = 3.14159265359;

float spaceship_x, spaceship_y, spaceship_color;
float spaceship_direction, spaceship_direction_thrust, spaceship_thrust;

float distance_to(float x1, float y1, float x2, float y2) {
  float dx = (x1-x2), dy = (y1-y2);
  return sqrt(dx*dx + dy*dy);
}

void set_main_mode(int mode) {
  tft.fillScreen(cBlack);
  main_mode = mode;
  delay(400);
}

void reset_spaceship() {
  spaceship_x = WIDTH/2, spaceship_y = HEIGHT/2;
  spaceship_direction = 0;
  spaceship_thrust = 0;
}

void draw_thrust_hov(float thrust, float dir_thrust, int thrust_color, int dir_thrust_color) {
  int thrust_h, dir_thrust_w;

  // Thrust
  thrust_h = thrust * HEIGHT / THRUST_MULTIPLY;
  tft.drawFastVLine(0, HEIGHT-thrust_h, thrust_h, thrust_color);

  // Directional Thrust
  dir_thrust_w = abs((int)(dir_thrust * WIDTH)) / DIR_THRUST_MULTIPLY;
  if (dir_thrust < 0) {
    tft.drawFastHLine(WIDTH/2 - dir_thrust_w, 0, dir_thrust_w, dir_thrust_color);
  } else {
    tft.drawFastHLine(WIDTH/2, 0, dir_thrust_w, dir_thrust_color);
  }
}

void draw_starfield(long seed, int count, int erase) {
  int i, x, y, color;
  randomSeed(seed);
  for (i = 0; i < count; i++) {
    x = random(WIDTH);
    y = random(HEIGHT);
    if (random(2) == 0)
      color = cGray;
    else
      color = cDarkGray;
    if (erase == 1) color = bg_color;
    tft.drawPixel(x, y, color);
  }
}

void draw_spaceship(float x, float y, float thrust, float dir, int color, int bg_color, int thrust_color) {  
  int len = 4;
  int x1 = x+cos(dir+pi*2/3)*len, y1 = y+sin(dir+pi*2/3)*len;
  int x2 = x+cos(dir+pi*4/3)*len, y2 = y+sin(dir+pi*4/3)*len;
  int x3 = x+cos(dir)*len*1.5, y3 = y+sin(dir)*len*1.5;
  tft.fillTriangle(x1, y1,  x2, y2,  x3, y3,  bg_color);
  tft.drawTriangle(x1, y1,  x2, y2,  x3, y3,  color);
  tft.drawLine(
    x+cos(dir+pi)*len, y+sin(dir+pi)*len,
    x+cos(dir+pi)*(len+thrust*2), y+sin(dir+pi)*(len+thrust*2),
    thrust_color);
}

int title_flash = 0;
void mode_title() {
  int mb_x = 6;
  tft.setTextSize(2);
  tft.setTextColor(cRed);
  tft.setCursor(34, 20);
  tft.println("8 Bit");
  // Typesetting for "Makebelieve"
  tft.setTextColor(cBlue);
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
    tft.setTextColor(cGray);
  else
    tft.setTextColor(cWhite);
  tft.setCursor(30, 100);
  tft.println("Press Start");
  
  title_flash++;
  
  if (digitalRead(button_m) == HIGH) {
    set_main_mode(1);
  }
}

int cal_x = 0, cal_y = 0, cal_r = 25;
void mode_calibrate() {
  tft.drawFastVLine(cal_x, 0, HEIGHT, cBlack);
  tft.drawFastHLine(0, cal_y, WIDTH, cBlack);
  
  cal_x = (analogRead(paddle_r) * HEIGHT / ANALOG_MAX);
  cal_y = HEIGHT - (analogRead(paddle_l) * WIDTH / ANALOG_MAX);

  tft.drawCircle(WIDTH/2, HEIGHT, cal_r, cYellow);
  tft.drawFastHLine(WIDTH/2-cal_r, HEIGHT, cal_r*2, cWhite);
  tft.drawFastVLine(WIDTH/2, HEIGHT-cal_r, cal_r, cWhite);

  tft.drawFastVLine(cal_x, 0, HEIGHT, cRed);
  tft.drawFastHLine(0, cal_y, WIDTH, cBlue);  

  tft.setTextSize(1);
  tft.setTextColor(cGray);
  tft.setCursor(40, 50);
  tft.println("your ship");

  tft.setTextColor(cWhite);
  tft.setCursor(40, 62);
  tft.println("IS READY!");
  tft.setCursor(37, 107);
  tft.println("start the");
  tft.setCursor(46, 119);
  tft.println("engine");

  if (digitalRead(button_m) == HIGH ||
      (cal_y == HEIGHT && cal_x >= WIDTH/2-3 && cal_x <= WIDTH/2+3) ) {
    reset_spaceship();
    set_main_mode(2);
  }
   
  delay(30);
}

float distance_to_sun;
void mode_play() {
  draw_thrust_hov(spaceship_thrust, spaceship_direction_thrust, bg_color, bg_color);
  draw_starfield(starfield_seed, 21, 1);
  draw_spaceship(
    spaceship_x, spaceship_y,
    spaceship_thrust, spaceship_direction,
    bg_color, bg_color, bg_color);  
  
  if (digitalRead(button_m) == HIGH) {
    set_main_mode(1);
  }

  spaceship_direction_thrust = DIR_THRUST_MULTIPLY *
    ((float)(analogRead(paddle_r) - ANALOG_MAX/2) / ANALOG_MAX);

  if (spaceship_direction_thrust < 0.025 &&
      spaceship_direction_thrust > -0.025) {
    spaceship_direction_thrust = 0.0;
  }
  spaceship_direction += spaceship_direction_thrust;  
  spaceship_thrust = (float)(analogRead(paddle_l) * THRUST_MULTIPLY) / ANALOG_MAX;
  if (spaceship_thrust < 0.5) spaceship_thrust = 0.0;
  
  spaceship_x += cos(spaceship_direction)*spaceship_thrust;
  spaceship_y += sin(spaceship_direction)*spaceship_thrust;
  
  distance_to_sun = distance_to(spaceship_x, spaceship_y, WIDTH, HEIGHT);
  if (distance_to_sun < 100) {
    float red = (1.0 - (distance_to_sun-50)/50);
    if (red > 1.0) red = 1.0;
    spaceship_color = tft.Color565(red*255, 0, 0);
  } else {
    spaceship_color = cBlack;
  }

  // Planets
  tft.drawCircle(10, HEIGHT-10, 8, cRed); // mars
  tft.drawCircle(WIDTH-10, 10, 6, cBlue); // earth
  tft.drawCircle(30, 10, 6, cGreen); // venus
  tft.drawCircle(WIDTH/2+10, HEIGHT/2, 12, cMagenta); // jupiter
  
  // Sun
  tft.drawCircle(WIDTH, HEIGHT, 28, cYellow);

  draw_starfield(starfield_seed, 21, 0);
  draw_spaceship(
    spaceship_x, spaceship_y,
    spaceship_thrust, spaceship_direction,
    cWhite, spaceship_color, cRed);
  
  draw_thrust_hov(spaceship_thrust, spaceship_direction_thrust, cDarkRed, cDarkBlue);

  delay(30);
}

void setup(void) {
  starfield_seed = analogRead(0);
  
  pinMode(button_l, INPUT);
  pinMode(button_r, INPUT);
  pinMode(button_m, INPUT);
  
  tft.initR(INITR_REDTAB);   // alternative: INITR_GREENTAB
  tft.fillScreen(cBlack);
  delay(500);
}

void loop() {
  switch(main_mode) {
    case 0: mode_title(); break;
    case 1: mode_calibrate(); break;
    case 2: mode_play(); break;
  }
}


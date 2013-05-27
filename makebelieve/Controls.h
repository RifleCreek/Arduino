#ifndef CONTROLS_H
#define CONTROLS_H

#define ANALOG_MAX 1024

class InputControl {
protected:
  int _pin;

public:
  InputControl(int pin);
};

class PaddleControl : InputControl {
public:
  PaddleControl(int pin);
  float value(float max_value);
  float value(float min_value, float max_value);
};

class ButtonControl : InputControl {
public:
  ButtonControl(int pin);
  bool is_pressed(void);
};

#endif
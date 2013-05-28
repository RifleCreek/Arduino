#include "Controls.h"
#include <Arduino.h>

InputControl::InputControl(int pin) { _pin = pin; }

PaddleControl::PaddleControl(int pin) : InputControl(pin) {}
ButtonControl::ButtonControl(int pin) : InputControl(pin) {
  pinMode(pin, INPUT);
}

float PaddleControl::value(float max_value) {
  float read_value = (float)(analogRead(_pin) * max_value) / ANALOG_MAX;
  if (read_value < 0) read_value = 0;
  if (read_value > max_value) read_value = max_value;
  return read_value;
}

float PaddleControl::value(float min_value, float max_value) {
  return value(max_value - min_value) + min_value;
}

bool ButtonControl::is_pressed(void) {
  return digitalRead(_pin) == HIGH;
}

#include <Controls.h>
#include <Arduino.h>

InputControl::InputControl(int pin) { _pin = pin; }

PaddleControl::PaddleControl(int pin) : InputControl(pin) {}
ButtonControl::ButtonControl(int pin) : InputControl(pin) {
  pinMode(pin, INPUT);
}

float PaddleControl::value(float max_value) {
  return (float)(analogRead(_pin) * max_value) / ANALOG_MAX;
}

bool ButtonControl::is_pressed(void) {
  return digitalRead(_pin) == HIGH;
}

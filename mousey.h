#pragma once
#include <math.h>
#include <array>

class Mousey {
  float direction;
  float x;
  float y;

public:
  static constexpr float PI = 4.0 * atan(1);
  static constexpr float HALF_PI = PI / 2.0;
  static constexpr float DEGREE = PI / 180.0;

  Mousey();
  Mousey(float x_, float y_, float direction_ = 0.0f);

  void move(float l);
  void turn(float angle);
  float getX();
  float getY();
};

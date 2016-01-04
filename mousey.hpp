#pragma once

#include "math.h"

struct Mousey {
  static constexpr float PI      = 4.0 * atan(1);
  static constexpr float HALF_PI = PI / 2.0;
  static constexpr float DEGREE  = PI / 180.0;

  float direction;

  float x;
  float y;

  Mousey() : x(), y(), direction() {}
  Mousey(float x_, float y_, float direction_ = 0.0f ) : x(x_), 
							 y(y_), 
							 direction(direction_) {}

  void move( float l ) {
    x += l * cos(direction);
    y += l * sin(direction);
  };

  void turn( float angle) {
    direction += angle;
  };

  inline float getX() { return x; };
  inline float getY() { return y; };
};

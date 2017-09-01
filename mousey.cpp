#include "mousey.h"

Mousey::Mousey() : x(), y(), direction() {}
Mousey::Mousey(float x_, float y_, float direction_)
    : x(x_), y(y_), direction(direction_) {}

void Mousey::move(float l) {
  x += l * cos(direction);
  y += l * sin(direction);
};

void Mousey::turn(float angle) { direction += angle; };

float Mousey::getX() { return x; };

float Mousey::getY() { return y; };

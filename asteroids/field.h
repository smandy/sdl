#pragma once

#include "math.h"
#include <cstdint>
#include <SDL.h>

namespace asteroids {
struct Field {
  SDL_Point sloc;
  uint16_t theta;

  static constexpr float DEGREE_TO_RADIAN = M_PI / 180;

  void draw(SDL_Renderer *renderer, SDL_Window *window);
  
  static constexpr uint32_t WIDTH = 800;
  static constexpr uint32_t HEIGHT = 800;
  
  Field();
};
  
}

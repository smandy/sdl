#pragma once

#include <SDL.h>
#include <vector>

#include "./field.h"
#include <functional>

using namespace asteroids;

class Game {
  SDL_Renderer *renderer;
  SDL_Window *window;

public:
  Game();

  void draw();

  void run();

  void process_input_events();

  bool game_running;
  bool running;

  void on_timer(uint32_t interval);

  Field f;
};

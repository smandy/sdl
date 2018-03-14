#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "./field.h"
#include <functional>

using namespace asteroids;

class Game {
  SDL_Renderer *renderer;
  SDL_Window *window;

public:
  Game();
  ~Game();

  void run();

  void process_input_events();

  bool game_running;
  bool running;
  bool gui;
  bool show_ctrl;

  void on_timer(uint32_t interval);

  Field f;
};

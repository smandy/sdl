#pragma once

#include <SDL.h>
#include <vector>

#include <functional>
#include "automata.h"

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

    bool gui_visible;

    Automata automata;

  void on_timer(uint32_t interval);
};

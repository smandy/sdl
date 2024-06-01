#pragma once

#include <SDL.h>
#include <vector>

#include "./field.h"
#include <functional>

#include "imgui.h"

using namespace asteroids;

class Game {
  SDL_Renderer *renderer;
  SDL_Window *window;

public:
  Game();
  ~Game();

  void run();

  void maybe_show_controls();

  void process_input_events(SDL_Event &event);

  bool game_running;
  bool running;
  bool gui;
  bool show_ctrl;

  ImVec4 clear_color;

  SDL_GLContext gl_context;
  // ImGuiIO *io;

  void on_timer(uint32_t interval);

  Field f;
};

#pragma once

#include <SDL.h>
#include <vector>

#include "tetronimo.h"
#include "well.h"

class Game {
  static constexpr int BLOCK_SIZE = 30;

  SDL_Renderer *renderer;
  SDL_Window *window;

  Well w;

  int t_idx;
  int offset_x;
  int offset_y;

public:
  Game();

  void run();
  void draw();

  void draw(Tetronimo &, int x, int y);
  void draw_block(int x, int y);
    
  static std::vector<SDL_Color> colors;
};

#pragma once

#include <SDL.h>
#include <vector>

#include "arena.h"
#include "types.h"
#include <functional>

namespace snake {
class Game {

public:

private:
  SDL_Renderer *renderer;
  SDL_Window *window;

public:
  Game();
  
  Arena a;
  void draw();
  void run();
  void process_input_events();

  bool paused;

  bool running;
  bool is_on_grid( const Coord&);
  void on_timer(uint32_t interval);

  // Snake specific
  void draw_block(const Coord &coord);
  void set_block_ul(const Coord &coord, SDL_Rect &r);
};
}

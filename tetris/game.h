#pragma once

#include <SDL.h>
#include <vector>

#include "tetronimo.h"
#include "well.h"
#include <functional>

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

  void draw(const Tetronimo &, int y, int x, bool outline);

  void draw_block(int x, int y, bool outline);
  void draw_block_filled(int x, int y);
  void draw_block_transparent(int x, int y);

  void on_timer(uint32_t interval);
  void init_tetr();
  void set_tetr_color(uint32_t i);
  bool can_move_to(const Tetronimo &z, uint8_t y, uint8_t x);
  void set_block_ul(int8_t y, int8_t x, SDL_Rect &r);

  void process_input_events();

  bool running;

  uint8_t tetr;
  int8_t tx;
  int8_t ty;
  uint8_t rot;

  bool game_running;

  const Tetronimo &t();
  void draw_well();
  bool show_periodic_table;
  static std::vector<SDL_Color> colors;

  bool down_impl();

  bool antigravity;

  bool check_rows();
  bool check_rows_impl();
  bool check_autofill();
};

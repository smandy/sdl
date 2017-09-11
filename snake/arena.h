#pragma once

#include <cstdint>
#include <deque>
#include <tuple>
#include <vector>

#include "types.h"

namespace snake {

struct Arena {

  static std::vector<Coord> directions;

  Arena();

  void on_timer();

  void draw();

  std::deque<Coord> segments;

  Coord direction;

  void move_snake();
  void randomize_m();

  GridType grid;
  RowType empty_row;
  bool game_running;
  void maybe_switch_direction( const Coord& new_direction);

  Coord m;
  CellType grid_value(const Coord&);
  void set_point( const Coord&, CellType);
};
}

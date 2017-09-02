#pragma once

#include <array>

#include "tetronimo.h"

struct Well {
  static constexpr int WIDTH = 10;
  static constexpr int HEIGHT = 20;

  using CellType = int;
  using LineType = std::array<CellType, WIDTH>;
  using RowsType = std::array<LineType, HEIGHT>;

  LineType empty_row;

  Well();
  
  void reset();

  bool collides(const Tetronimo &t, int8_t x, int8_t y);

  bool in_well(const Tetronimo &t, int8_t x, int8_t y);

  void place(const Tetronimo &t, int8_t x, int8_t y, int8_t col);

  RowsType rows;
};

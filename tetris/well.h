#pragma once

#include <array>

class Well {
public:
  static constexpr int WIDTH = 12;
  static constexpr int HEIGHT = 30;

  using CellType = int;
  using LineType = std::array<CellType, WIDTH>;
  using RowsType = std::array<LineType, HEIGHT>;

  Well();

private:
  RowsType rows;
};

#pragma once

#include <array>
#include <vector>

struct Tetronimo;

using TArray = std::array<Tetronimo, 4>;
using TArrayVec = std::vector<TArray>;

struct Tetronimo {
  using DataType = std::array<std::array<bool, 4>, 4>;
  DataType _data;

  Tetronimo();
  Tetronimo(const char *data);
  static const TArrayVec tetronimos;
  Tetronimo rotate() const;
};

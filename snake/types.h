#pragma once

#include <tuple>
#include <cstdint>
#include <complex>

static constexpr uint8_t BLOCK_SIZE = 30;
static constexpr uint8_t X_BLOCKS = 30;
static constexpr uint8_t Y_BLOCKS = 20;

using Coord = std::complex<int8_t>;

enum class CellType : uint8_t {
  EMPTY = 0,
  SNAKE,
  MUSHROOM
};

using RowType = std::array< CellType, X_BLOCKS >;
using GridType = std::array< RowType, Y_BLOCKS >;


#include "arena.h"
#include "game.h"

#include <iostream>
#include <string>

namespace snake {
Arena::Arena()
    : segments{}, direction{directions[rand() % 4]}, grid{}, game_running{
                                                                 true} {
  segments.push_back({X_BLOCKS / 2, Y_BLOCKS / 2});
  empty_row.fill(CellType::EMPTY);
  grid.fill(empty_row);
  for (int i = 0; i < 10; ++i) {
    randomize_m();
  }
}

std::ostream &operator<<(std::ostream &os, Coord &c) {
  os << "{" << std::to_string(c.real()) << "," << std::to_string(c.imag())
     << "}";
  return os;
}

void Arena::randomize_m() {
  Coord m;
  do {
    m.real(rand() % X_BLOCKS);
    m.imag(rand() % Y_BLOCKS);
  } while (grid_value(m) != CellType::EMPTY);
  set_point(m, CellType::MUSHROOM);
}

CellType Arena::grid_value(const Coord &c) { return grid[c.imag()][c.real()]; };

void Arena::maybe_switch_direction(const Coord &new_direction) {
  auto new_point = segments.back() + new_direction;
  if (grid[new_point.imag()][new_point.real()] == CellType::EMPTY) {
    direction = new_direction;
  };
}

bool is_on_grid(const Coord &x) {
  if (x.real() < 0)
    return false;
  if (x.imag() < 0)
    return false;
  if (x.real() >= X_BLOCKS)
    return false;
  if (x.imag() >= Y_BLOCKS)
    return false;
  return true;
}

void Arena::move_snake() {
  // std::cout << "move snake back is " << segments.back() << std::endl;
  auto new_point = segments.back() + direction;
  if (!is_on_grid(new_point)) {
    game_running = false;
    return;
  }

  // std::cout << "Value is " << std::to_string( (uint8_t)grid_value(new_point))
  // << std::endl;
  auto prev = grid_value(new_point);

  if (grid_value(new_point) == CellType::SNAKE) {
    game_running = false;
    return;
  }

  segments.push_back(new_point);
  set_point(new_point, CellType::SNAKE);

  if (prev == CellType::MUSHROOM) {
    // Not removing end segmnt implies growth.
    randomize_m();
  } else {
    auto x = segments.front();
    segments.pop_front();
    set_point(x, CellType::EMPTY);
  }
}

void Arena::set_point(const Coord &c, CellType t) {
  grid[c.imag()][c.real()] = t;
};

std::vector<Coord> Arena::directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
}

#include "arena.h"
#include "game.h"

#include <iostream>

namespace snake {
Arena::Arena()
    : segments{}, direction{directions[rand() % 4]}, grid{}, game_running{
                                                                 true} {
  segments.push_back({X_BLOCKS / 2, Y_BLOCKS / 2});
  randomize_m();
  empty_row.fill(CellType::EMPTY);
  grid.fill(empty_row);
};
std::ostream &operator<<(std::ostream &os, Coord &c) {
  os << "{" << std::to_string(c.real()) << "," << std::to_string(c.imag())
     << "}";
  return os;
};

void Arena::randomize_m() {
  m.real(rand() % X_BLOCKS);
  m.imag(rand() % Y_BLOCKS);
};

void Arena::maybe_switch_direction(const Coord &new_direction) {
  auto new_point = segments.back() + new_direction;
  if (grid[new_point.imag()][new_point.real()] == CellType::EMPTY) {
    direction = new_direction;
  };
};

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
};

void Arena::move_snake() {
  // std::cout << "move snake back is " << segments.back() << std::endl;
  auto new_point = segments.back() + direction;

  if (!is_on_grid(new_point)) {
    game_running = false;
  };

  if (grid[new_point.imag()][new_point.real()] == CellType::SNAKE) {
    game_running = false;
  };
  segments.push_back(new_point);
  grid[new_point.imag()][new_point.real()] = CellType::SNAKE;
  if (new_point == m) {
    randomize_m();
  } else {
    auto x = segments.front();
    segments.pop_front();
    grid[x.imag()][x.real()] = CellType::EMPTY;
  }
};

std::vector<Coord> Arena::directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
};

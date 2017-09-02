#include "well.h"
#include <cassert>
#include <iostream>

Well::Well() {
  empty_row.fill(-1);
  reset();
}

void Well::reset() {
  rows.fill(empty_row);
}

bool Well::in_well(const Tetronimo &t, int8_t y, int8_t x) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (!t._data[j][i])
        continue;
      uint8_t yy = y + j;
      uint8_t xx = x + i;

      if (xx < 0 || xx >= WIDTH)
        return false;
      if (yy < 0 || yy >= HEIGHT)
        return false;
    }
  }
  return true;
}

void Well::place(const Tetronimo &t, int8_t y, int8_t x, int8_t col) {
  assert(in_well(t, y, x));
  assert(!collides(t, y, x));
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (!t._data[j][i])
        continue;
      uint8_t yy = y + j;
      uint8_t xx = x + i;
      rows[yy][xx] = col;
    }
  }
}

bool Well::collides(const Tetronimo &t, int8_t y, int8_t x) {
  assert(in_well(t, y, x));
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (!t._data[j][i])
        continue;
      uint8_t yy = y + j;
      uint8_t xx = x + i;
      if (rows[yy][xx] != -1)
        return true;
    }
  }
  return false;
}


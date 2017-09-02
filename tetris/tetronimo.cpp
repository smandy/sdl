#include "tetronimo.h"
#include <iostream>

Tetronimo::Tetronimo() : _data{} {}

Tetronimo::Tetronimo(const char *data) : _data{} {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (data[i * 4 + j] != ' ') {
        _data[i][j] = true;
      }
    }
  }
}

Tetronimo Tetronimo::rotate() {
  Tetronimo ret;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (_data[i][j]) {
        ret._data[j][3 - i] = true;
      }
    }
  }
  return ret;
}

TArrayVec make_array(std::vector<Tetronimo> &&ts) {
  TArrayVec ret;
  for (Tetronimo t : ts) {
    TArray row;
    for (int i = 0; i < 4; ++i) {
      row[i] = t;
      t = t.rotate();
    }
    ret.push_back(row);
  }
  return ret;
}

// clang-format off
TArrayVec Tetronimo::tetronimos = make_array({
    "    "
    "    "
    "****"
    "    ",

    "    "
    "*** "
    "  * "
    "    ",

    "    "
    " ** "
    " ** "
    "    ",

    "    "
    " ** "
    "**  "
    "    ",

    "    "
    "*** "
    " *  "
    "    ",

    "    "
    "**  "
    " ** "
    "    ",
});

// clang-format on

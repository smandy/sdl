#pragma once

class Well {

public:
  static constexpr int WIDTH = 12;
  static constexpr int HEIGHT = 30;

  Well();

private:
  int rows[HEIGHT][WIDTH];
};

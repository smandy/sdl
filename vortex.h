#pragma once
#include <SDL.h>

#include <vector>

class Vortex {
  SDL_Renderer *renderer;
  SDL_Window *window;
  std::vector<SDL_Point> myVec;

public:
  Vortex();

  void drawVortex(float degreeOffset, float lengthFraction);

  void drawScene(float degreeOffset, float lengthFraction);

  void run();
};

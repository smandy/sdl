#include "field.h"

#include <algorithm>
#include <array>
#include <vector>

#include "SDL.h"

namespace asteroids {
Field::Field() : theta {} {
  sloc.x = WIDTH / 2;
  sloc.y = HEIGHT / 2;
};

struct Polar {
  int32_t theta;
  uint32_t r;

  Polar(int32_t _theta, uint32_t _r) : theta(_theta), r(_r){};
};

void Field::draw(SDL_Renderer *renderer, SDL_Window *window) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

  std::vector<Polar> points = {
      {0, 100}, {135, 50}, {180, 20}, {-135, 50}, {0, 100}};
  std::array<SDL_Point, 5> sdl_points;

  std::transform(points.begin(), points.end(), sdl_points.begin(), [&](auto &x) {
      return SDL_Point { int(sloc.x + x.r * cos(DEGREE_TO_RADIAN * (x.theta + theta))),
          int(sloc.y + x.r * sin(DEGREE_TO_RADIAN * (x.theta + theta)))};
  });

  SDL_RenderDrawLines(renderer, sdl_points.data(), sdl_points.size());
  SDL_RenderPresent(renderer);
}}

#include "field.h"

#include <algorithm>
#include <array>
#include <vector>

#include "SDL.h"
#include <iostream>
#include <math.h>

namespace asteroids {
Field::Field() : theta{} {};
  void Field::apply_motion() {
   for( auto& x : entities) {
     x.position += x.velocity; 
   };
 };
  
void Field::draw(SDL_Renderer *renderer, SDL_Window *window) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

  std::vector<Vec2> points = {{100, 0},
                              {40, 5 * M_PI / 8},
                              {20, 2 * M_PI},
                              {40, -5 * M_PI / 8},
                              {100, 0}};
  std::array<SDL_Point, 5> sdl_points;

  static constexpr float scale = 0.3;
  std::transform(points.begin(),
                 points.end(),
                 sdl_points.begin(),
                 [&](auto &x) {
                   auto sloc = entities[SHIP_ID].position;
                   auto rel = std::polar(x.real(), x.imag() + theta);
                   //std::cout << "rel is " << rel << std::endl;
                   SDL_Point ret;
                   to_point(sloc + scale * rel, ret);
                   return ret;
                 });
  SDL_RenderDrawLines(renderer, sdl_points.data(), sdl_points.size());
  SDL_RenderPresent(renderer);
}
}

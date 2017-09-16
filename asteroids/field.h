#pragma once

#include "math.h"
#include "types.h"
#include <SDL.h>
#include <array>
#include <cstdint>

namespace asteroids {
struct Component {
  bool active;
  
  Vec2 position;
  Vec2 velocity;

  Component()
      : active{false}, position{WIDTH / 2, HEIGHT / 2},
        velocity{} {}
};

struct Bullet {
  uint8_t ttl;
  Bullet() : ttl{10} {};
};

struct Field {
  float theta;

  std::array<Component, ENTITY_SIZE> entities;
  std::array<Bullet, MAX_BULLETS> bullets;

  void launch_bullet();
  void draw(SDL_Renderer *renderer, SDL_Window *window);

  void apply_motion();

  Field();
};
}

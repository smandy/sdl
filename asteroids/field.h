#pragma once

#include "math.h"
#include "types.h"
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <vector>

namespace asteroids {
struct Component {
  bool active;

  Vec2 position;
  Vec2 velocity;

  Component() : active{false}, position{WIDTH / 2, HEIGHT / 2}, velocity{} {}
};

struct Bullet {
  uint8_t ttl;
  uint32_t entity_id;
  Bullet() : entity_id{} { reset(); };
  void reset() { ttl = 100; };
};

struct Asteroid {
  Asteroid(uint32_t entity_id_, std::vector<Vec2> &&coords_, float rot_)
      : entity_id(entity_id_), coords(coords_), rot(rot_){};
  uint32_t entity_id;
  std::vector<Vec2> coords;
  float theta;
  float rot;
};

struct Field {
  float theta;

  // Bullet state
  size_t front;
  size_t back;

  std::array<Component, ENTITY_SIZE> entities;
  std::vector<Bullet> bullets;
  std::vector<Bullet> bullet_pool;

  void launch_bullet();
  void draw(SDL_Renderer *renderer);
  void handle_bullets();

    
  void update_state();

  void apply_motion();
  void fire_bullet();

  void draw_ship(SDL_Renderer *renderer);
  void draw_bullets(SDL_Renderer *renderer);
  void draw_asteroid(SDL_Renderer *renderer, Asteroid &);

  std::vector<Asteroid> asteroids;

  // std::vector<std::vector<Vec2>> asteroids;
  // std::vector<float> thetas;
  // std::vector<float> rots;

  Field();
};
}

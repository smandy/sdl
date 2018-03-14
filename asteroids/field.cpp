#include "field.h"

#include <algorithm>
#include <array>
#include <vector>

#include "SDL2/SDL.h"
#include <iostream>
#include <math.h>
#include "assert.h"

namespace asteroids {
Field::Field() : theta{}, front{}, back{}, bullets{}, bullet_pool(MAX_BULLETS) {
  entities[SHIP_ID].active = true;

  auto entity_id = BULLET_OFFSET;
  for (auto &x : bullet_pool) {
    x.entity_id = entity_id++;
  }

  for (int i = 0; i < MAX_ASTEROIDS; ++i) {
    uint32_t entity_id = ASTEROID_OFFSET + i;
    auto rot = DEGREE_TO_RADIAN * (-5 + (rand() % 10));
    auto &x = entities[entity_id];
    x.active = true;
    x.position.real(rand() % WIDTH);
    x.position.imag(rand() % WIDTH);
    x.velocity.real(rand() % 5 - 3);
    x.velocity.imag(rand() % 5 - 3);

    std::vector<Vec2> points;
    for (int i = 0; i < NUM_POINTS; ++i) {
      auto randtheta = DEGREE_TO_RADIAN * (rand() % 20);
      points.push_back({100.0f + rand() % 100,
                        float(randtheta + (i * 2 * M_PI) / NUM_POINTS)});
      assert( points[0].imag() < (2 * M_PI) );
    }
    points.push_back(points[0]);
    asteroids.push_back( {entity_id, std::move(points), rot} );
  }
}

void Field::update_state() {
  apply_motion();
  handle_bullets();
}

void Field::apply_motion() {
  for (auto &x : entities) {
    if (!x.active) {
      continue;
    }
    x.position += x.velocity;
    if (x.position.real() < 0) {
      x.position.real(x.position.real() + WIDTH);
    }
    if (x.position.real() > WIDTH) {
      x.position.real(x.position.real() - WIDTH);
    }
    if (x.position.imag() < 0) {
      x.position.imag(x.position.imag() + HEIGHT);
    }
    if (x.position.imag() > HEIGHT) {
      x.position.imag(x.position.imag() - HEIGHT);
    }
  }

  for (auto &x : asteroids) {
    x.theta += x.rot;
  }
}

void Field::fire_bullet() {
  if (!bullet_pool.empty()) {
    bullets.emplace_back(bullet_pool.back());
    bullet_pool.pop_back();
    Vec2 head_of_ship = {100, 0};
    auto sloc = entities[SHIP_ID].position;
    auto rel = std::polar(head_of_ship.real(), head_of_ship.imag() + theta);
    auto tip_of_ship = sloc + Constants::SHIP_SCALE * rel;
    auto velocity = entities[SHIP_ID].velocity;
    auto dv = std::polar(10.0f, theta);
    auto bullet_velocity = velocity + dv;
    auto &b = bullets.back();
    b.reset();
    entities[b.entity_id].active = true;
    entities[b.entity_id].position = tip_of_ship;
    entities[b.entity_id].velocity = bullet_velocity;
    // std::cout << "Entity id is " << b.entity_id << std::endl;
    ++front;
  }
}

void Field::handle_bullets() {
  auto change = false;
  for (auto &b : bullets) {
    --b.ttl;
    // std::cout << "Ttl is " << std::to_string(b.ttl) << std::endl;
    if (!b.ttl) {
      // std::cout << "Retiring entity " << b.entity_id << std::endl;
      entities[b.entity_id].active = false;
      bullet_pool.emplace_back(b);
      change = true;
    }
  }

  if (change) {
    bullets.erase(std::remove_if(std::begin(bullets), std::end(bullets),
                                 [](auto &b) { return !b.ttl; }));
  }
}

void Field::draw(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
  draw_ship(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
  draw_bullets(renderer);

  std::for_each(std::begin(asteroids), std::end(asteroids),
                [&renderer, this](auto &d) { draw_asteroid(renderer, d); });
}

template <typename T> struct TR;

void Field::draw_bullets(SDL_Renderer *renderer) {
  SDL_Rect r = {0, 0, Constants::BULLET_WIDTH, Constants::BULLET_WIDTH};
  static std::vector<SDL_Rect> rects(MAX_BULLETS);
  Vec2 offset = { - Constants::BULLET_WIDTH / 2.0f, -Constants::BULLET_WIDTH / 2.0f };
  //Vec2 offset = 
  rects.clear();
  for (auto &x : bullets) {
    auto &entity = entities[x.entity_id];
    // TR<decltype(&entity.position)> fail;
    to_point(entity.position + offset , r);
    rects.push_back(r);
  }
  SDL_RenderFillRects(renderer, rects.data(), rects.size());
}

void Field::draw_asteroid(SDL_Renderer *renderer, Asteroid &d) {
  // std::cout << "Drawing " << id << std::endl;
  // for( auto& x : points) {
  //   std::cout << x << std::endl;
  // };
  std::array<SDL_Point, NUM_POINTS + 1> sdl_points;
  std::transform(d.coords.begin(), d.coords.end(), sdl_points.begin(),
                 [&](auto &x) {
                   auto sloc = entities[d.entity_id].position;
                   auto rel = std::polar(x.real(), x.imag() + d.theta);
                   // std::cout << "rel is " << rel << std::endl;
                   SDL_Point ret;
                   to_point(sloc + Constants::ASTEROID_SCALE * rel, ret);
                   return ret;
                 });
  SDL_RenderDrawLines(renderer, sdl_points.data(), sdl_points.size());
}

void Field::draw_ship(SDL_Renderer *renderer) {
  std::vector<Vec2> points = {{100, 0},
                              {40, 5 * M_PI / 8},
                              {20, 2 * M_PI},
                              {40, -5 * M_PI / 8},
                              {100, 0}};
  std::array<SDL_Point, 5> sdl_points;
  std::transform(points.begin(), points.end(), sdl_points.begin(),
                 [&](auto &x) {
                   auto sloc = entities[SHIP_ID].position;
                   auto rel = std::polar(x.real(), x.imag() + theta);
                   // std::cout << "rel is " << rel << std::endl;
                   SDL_Point ret;
                   to_point(sloc + Constants::SHIP_SCALE * rel, ret);
                   return ret;
                 });
  SDL_RenderDrawLines(renderer, sdl_points.data(), sdl_points.size());
}
}

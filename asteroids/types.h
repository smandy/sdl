#pragma once

#include <SDL.h>
#include <complex>
#include <cstdint>

namespace asteroids {

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::complex<T> &t) {
  os << "{" << t.real() << "," << t.imag() << "}";
  return os;
};

using Vec2 = std::complex<float>;

inline void to_point(Vec2 &&vec, SDL_Point &p) {
  p.x = (int)vec.real();
  p.y = (int)vec.imag();
};

static constexpr uint32_t WIDTH = 800;
static constexpr uint32_t HEIGHT = 800;

static constexpr float DEGREE_TO_RADIAN = M_PI / 180;

static constexpr uint8_t SHIP_ID = 0;
static constexpr uint8_t ALIEN_ID = 1;

static constexpr uint8_t MAX_BULLETS = 5;
static constexpr uint8_t MAX_ASTEROIDS = 5;

static constexpr uint8_t BULLET_OFFSET = ALIEN_ID + 1;
static constexpr uint8_t ASTEROID_OFFSET = BULLET_OFFSET + MAX_BULLETS;
static constexpr uint8_t ENTITY_SIZE = ASTEROID_OFFSET + MAX_ASTEROIDS;
}

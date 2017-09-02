#include "game.h"

#include <SDL.h>
#include <iostream>

Game::Game() : t_idx{}, offset_x{6}, offset_y{4} {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }
  window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, BLOCK_SIZE * Well::WIDTH,
                            BLOCK_SIZE * Well::HEIGHT, SDL_WINDOW_SHOWN);

  if (!window) {
    std::cout << "Error creating window " << SDL_GetError() << std::endl;
    exit(1);
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    std::cout << "Error creating renderer " << SDL_GetError() << std::endl;
    exit(1);
  }
}

void Game::draw(Tetronimo &t, int x, int y) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (t._data[i][j]) {
        draw_block(x + i, y + j);
      }
    }
  }
}

void Game::draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
  for (int i = 0; i < Well::WIDTH; i++) {
    for (int j = 0; j < Well::HEIGHT; j++) {
      auto x = i * BLOCK_SIZE;
      auto y = j * BLOCK_SIZE;
      SDL_RenderDrawLine(renderer, x - 3, y, x + 3, y);
      SDL_RenderDrawLine(renderer, x, y - 3, x, y + 3);
    }
  }

  int x = offset_x;
  int y = offset_y;
  for (int j = 0; j < Tetronimo::tetronimos.size(); ++j) {
    x = offset_x;
    SDL_Color &c = colors[j];
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    for (int i = 0; i < 4; ++i) {
      Tetronimo &t = Tetronimo::tetronimos[j][i];
      draw(t, x, y);
      x += 5;
    }
    y += 5;
  }
  SDL_RenderPresent(renderer);
}

void Game::draw_block(int x, int y) {
  SDL_Rect r;
  r.x = x * BLOCK_SIZE;
  r.y = y * BLOCK_SIZE;
  r.w = BLOCK_SIZE - 2;
  r.h = BLOCK_SIZE - 2;
  SDL_RenderFillRect(renderer, &r);
}

void Game::run() {
  bool done{false};
  float degreeOffset = 2.0f;
  float lengthFraction = 0.98;

  bool keyDown = false;

  while (!done) {
    SDL_Event event;
    draw();
    typedef decltype(event.type) lastEvent;
    int haveEvent = SDL_PollEvent(&event);
    if (haveEvent) {
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          done = true;
          break;
        case SDLK_SPACE:
          t_idx = (t_idx + 1) % Tetronimo::tetronimos.size();
          break;
        case SDLK_LEFT:
          --offset_x;
          break;
        case SDLK_RIGHT:
          ++offset_x;
          break;
        case SDLK_UP: {
          --offset_y;
          break;
        }
        case SDLK_DOWN:
          ++offset_y;
          break;
        }
      }
      if (event.type == SDL_QUIT) {
        done = true;
      }
    }
  }
  SDL_Quit();
}

std::vector<SDL_Color> Game::colors = {
    {0, 255, 255, 0}, {0, 0, 255, 0},   {255, 165, 0, 0}, {255, 255, 0, 0},
    {0, 255, 255, 0}, {255, 0, 255, 0}, {255, 0, 0, 0}};

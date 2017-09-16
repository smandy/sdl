#include "game.h"

#include <SDL.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

uint32_t my_timer_func(uint32_t interval, void *ctx) {
  // std::cout << "My Timer" << std::endl;
  SDL_Event event;
  SDL_UserEvent userevent;
  /* In this example, our callback pushes an SDL_USEREVENT event
     into the queue, and causes our callback to be called again at the
     same interval: */
  userevent.type = SDL_USEREVENT;
  userevent.code = 1;
  userevent.data1 = ctx;
  userevent.data2 = (void *)(size_t)interval;
  event.type = SDL_USEREVENT;
  event.user = userevent;
  SDL_PushEvent(&event);
  return interval;
}

Game::Game() : game_running{true}, running{true} {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }
  window =
      SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       Field::WIDTH, Field::HEIGHT, SDL_WINDOW_SHOWN);

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
  uint32_t delay = 400; /* To round it down to the nearest 10 ms */
  SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);
}

void Game::draw() {
  static SDL_Rect r;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void Game::on_timer(uint32_t interval) {
}

void Game::run() {
  running = true;
  while (running) {
    f.draw(renderer, window);
    process_input_events();
  }
  SDL_Quit();
}

void Game::process_input_events() {
  SDL_Event event;
  uint32_t lastEvent;
  bool keyDown = false;
  int haveEvent = SDL_PollEvent(&event);
  if (haveEvent) {
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_r: {
        break;
      }
      case SDLK_ESCAPE: {
        running = false;
        break;
      }
      }
    }

    if (event.type == SDL_KEYDOWN && game_running) {
      switch (event.key.keysym.sym) {
      case SDLK_SPACE: {
        break;
      }
      case SDLK_p: {
        break;
      }
      case SDLK_a: {
        break;
      }
      case SDLK_n: {
        break;
      }
      case SDLK_RIGHT: {
        f.theta += 5;
        break;
      }
      case SDLK_LEFT: {
        f.theta -= 5;
      }
      case SDLK_UP: {
      }
      case SDLK_DOWN:
        break;
      }
    }

    if (event.type == SDL_QUIT) {
      running = false;
    }

    if (event.type == SDL_USEREVENT) {
      reinterpret_cast<Game *>(event.user.data1)
          ->on_timer(reinterpret_cast<size_t>(event.user.data2));
    }
  }
}

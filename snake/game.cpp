#include "game.h"

#include <SDL.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

namespace snake {
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

  Game::Game() : running{true}, paused{false}, a{} {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }
  window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, BLOCK_SIZE * (X_BLOCKS + 5),
                            BLOCK_SIZE * (Y_BLOCKS + 5), SDL_WINDOW_SHOWN);

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
  uint32_t delay = 100; /* To round it down to the nearest 10 ms */
  SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);
}

void Game::draw() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  
  SDL_SetRenderDrawColor(renderer, 255,255, 255, 0);
  for( int8_t x = -1;x<X_BLOCKS+1;++x) {
    draw_block( { x, -1 });
    draw_block( { x, Y_BLOCKS });
  };

  for( int8_t y = -1;y<Y_BLOCKS+1;++y) {
    draw_block( {  -1,y });
    draw_block( { X_BLOCKS,y} );
  };

  uint8_t c = 255;
  SDL_SetRenderDrawColor(renderer, c, 0, 0, 0);
  for (auto x = std::rbegin(a.segments); x!=std::rend(a.segments); ++x) {
    if (c>100) {
      c -=10;
      SDL_SetRenderDrawColor(renderer, c, 0, 0, 0);
    }
    draw_block(*x);
  };

  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

  // for (int8_t y = 0;y<Y_BLOCKS;++y) {
  //   for( int8_t x = 0;x<X_BLOCKS;++x) {
  //     if ( a.grid[y][x] != CellType::EMPTY) {
  //       draw_block( { x, y } );
  //     }
  //   };
  // };
  
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);
  draw_block(a.m);
  SDL_RenderPresent(renderer);
}

void Game::draw_block(const Coord &c) {
  static SDL_Rect r;
  set_block_ul(c, r);
  r.w = BLOCK_SIZE - 2;
  r.h = BLOCK_SIZE - 2;
  SDL_RenderFillRect(renderer, &r);
};

void Game::set_block_ul(const Coord &c, SDL_Rect &r) {
  r.x = c.real() * BLOCK_SIZE + 50;
  r.y = (c.imag() + 1) * BLOCK_SIZE + 50;
}
  
void Game::on_timer(uint32_t interval) {
  if (a.game_running && !paused) {
    a.move_snake();
  }
}

void Game::run() {
  running = true;
  while (running) {
    draw();
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
        a = Arena {};
        break;
      }
      case SDLK_p: {
        paused = !paused;
        break;
      }
        
      case SDLK_ESCAPE: {
        running = false;
        break;
      }
      }
    }
    
    if (event.type == SDL_KEYDOWN && a.game_running) {
      switch (event.key.keysym.sym) {
      case SDLK_SPACE: {
        break;
      }
      case SDLK_r: {
      }
      case SDLK_a: {
        break;
      }
      case SDLK_n: {
        break;
      }
      case SDLK_RIGHT: {
        a.maybe_switch_direction(Arena::directions[1]);
        break;
      }
      case SDLK_LEFT: {
        a.maybe_switch_direction(Arena::directions[3]);
        break;
      }
      case SDLK_UP: {
        a.maybe_switch_direction(Arena::directions[2]);
        break;
      }
      case SDLK_DOWN:
        a.maybe_switch_direction(Arena::directions[0]);
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
}

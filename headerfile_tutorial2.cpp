#include <SDL2/SDL.h>
#include <iostream>
#include "SDL_ttf.h"

#include <sstream>

constexpr int sizeX = 800;
constexpr int sizeY = 400;

enum class PaddleState {
  MOVING_LEFT,
  MOVING_RIGHT,
  STATIONARY
};

template <typename T> struct TD;

uint32_t cb2(uint32_t currentInterval, void *p) {
  SDL_Event event;
  SDL_UserEvent userevent;
  /* In this example, our callback pushes a function
  into the queue, and causes our callback to be called again at the
  same interval: */

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = 0;
  userevent.data2 = 0;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);
  return (currentInterval);
}

struct App {
  SDL_Renderer *renderer;
  SDL_Window *window;

  float x;
  float y;

  PaddleState paddleState = PaddleState::STATIONARY;
  int posX = 100;
  int posY = 200;
  int paddleX = 200;
  TTF_Font *font;

  float dx = 3.0;
  float dy = 3.0;

  int calls = 0;
  decltype(SDL_GetTicks()) numTicks;
  std::stringstream oss;
  bool firstPass = true;
    
  App() {
    // Initialize SDL
    // ==========================================================
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      // Something failed, print error and exit.
      std::cout << " Failed to initialize SDL : " << SDL_GetError()
                << std::endl;
      exit(1);
    }

    // Create and init the window
    // ==========================================================
    window = SDL_CreateWindow("Server", posX, posY, sizeX, sizeY, 0);

    if (window == nullptr) {
      std::cout << "Failed to create window : " << SDL_GetError();
      exit(1);
    }

    // Create and init the renderer
    // ==========================================================
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr) {
      std::cout << "Failed to create renderer : " << SDL_GetError();
      exit(1);
    }

    TTF_Init();
    if (font == nullptr) {
      font = TTF_OpenFont(
          "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
          12);
      if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
        exit(1);
      };
      std::cout << "Have set font " << font << std::endl;
    }

    // Render something
    // ==========================================================
    SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);
    auto x = SDL_AddTimer(20, &cb2, nullptr);
  };

  void updateState() {
    x += dx;
    y += dy;

    if (x > (sizeX - 30) || x <= 0) {
      dx = -dx;
    }

    if (y > (sizeY - 30) || y <= 0) {
      dy = -dy;
    }

    switch (paddleState) {
    case PaddleState::MOVING_LEFT: {
      if (paddleX > 10) {
        paddleX -= 10;
      }
      break;
    case PaddleState::MOVING_RIGHT: {
      if (paddleX < (sizeX - 10)) {
        paddleX += 10;
      };
      break;
    };
    };
    };
  };

  void render() {

    // TD<decltype(TTF_OpenFont("foo", 10))> fooType;

    // auto ctx = reinterpret_cast<Context *>(p);
    // std::cout << "TImer x=" << x << " y=" << y << "\n";
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderClear(renderer);

    SDL_Color c{ 255, 255, 255, 255 };
    auto thisTicks = SDL_GetTicks();
      auto diffTicks = (thisTicks - numTicks);
      auto fps = 1000.0 / diffTicks;
      oss.str("");
      oss.clear();
      oss << "Woot " << calls++ << " diffticks=" << diffTicks
          << " ticks=" << thisTicks << " fps=" << fps << " ms=" << diffTicks;
    numTicks = thisTicks;

    if (oss.str().size()) {
      auto surf2 = TTF_RenderText_Solid(font, oss.str().c_str(), c);
      if (!surf2) {
        printf("TTF_RenderText: %s\n", TTF_GetError());
      }

      auto texture = SDL_CreateTextureFromSurface(renderer, surf2);
      if (!texture) {
        printf("SDL_CreateTexturefromsurface: %s\n", SDL_GetError());
      };

      auto h = surf2->h;
      auto w = surf2->w;

      SDL_FreeSurface(surf2);
      SDL_Rect dest = { 20, 20, w, h };
      SDL_RenderCopy(renderer, texture, nullptr, &dest);
      SDL_DestroyTexture(texture);
    }

    // SDL_RenderClear( renderer );
    SDL_Rect r{ static_cast<int32_t>(x), static_cast<int32_t>(y), 25, 25 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &r);
    // Render the changes above ( which up until now had just happened behind
    // the
    // scenes )
    SDL_Rect paddleRect{ paddleX, sizeY - 10, 50, 10 };
    SDL_RenderDrawRect(renderer, &paddleRect);
    // std::cout << SDL_GetThreadName() << std::endl;
    SDL_RenderPresent(renderer);
  };

  void mainLoop() {
    SDL_Event event;
    bool done = false;

    static int eventNumber = 0;

    while (!done) {
      while (SDL_PollEvent(&event)) {
        auto tix = SDL_GetTicks();
        eventNumber++;
        switch (event.type) {
        case SDL_KEYDOWN: {
          if (!event.key.repeat) {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: {
              done = 1;
              break;
            }
            case SDLK_LEFT: {
              paddleState = PaddleState::MOVING_LEFT;
              break;
            }
            case SDLK_RIGHT: {
              paddleState = PaddleState::MOVING_RIGHT;
              break;
            }
            }
          }
          break;
        };
        case SDL_KEYUP: {
          if (!event.key.repeat) {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT: {
              paddleState = PaddleState::STATIONARY;
              break;
            }
            case SDLK_RIGHT: {
              paddleState = PaddleState::STATIONARY;
              break;
            }
            }
          };
          break;
        };
        case SDL_USEREVENT: {
          /* and now we can call the function we wanted to call in the timer but
           * couldn't because of the multithreading problems */
          // auto p = (void (*)(void *))event.user.data1;
          // p(event.user.data2);
          updateState();
          render();
          break;
        }
        case SDL_QUIT: {
          done = true;
          break;
        }
        }
        /* ... */
      }
    }
  }
};

int main(int argc, char *args[]) {
  App myApp;
  myApp.mainLoop();
}

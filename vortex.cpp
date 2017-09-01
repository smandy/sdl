#include <iostream>
#include <stdlib.h> //Needed for "exit" function

#include <SDL.h>
#include <iostream>
#include <vector>

#include "mousey.h"
#include "vortex.h"

using namespace std;

Vortex::Vortex() : myVec{} {
  /* Initialize SDL for video output */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  window =
      SDL_CreateWindow("Vortex", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1000, 1000, SDL_WINDOW_SHOWN);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
};

void Vortex::drawVortex(float degreeOffset, float lengthFraction) {
  float l = 1000;
  Mousey m(0, 0);
  myVec.clear();
  while (l > 0.05) {
    myVec.push_back({(int)m.getX(), (int)m.getY()});
    m.move(l);
    m.turn(Mousey::HALF_PI + degreeOffset * Mousey::DEGREE);
    l *= lengthFraction;
  };
  SDL_RenderDrawLines(renderer, myVec.data(), myVec.size());
};

void Vortex::drawScene(float degreeOffset, float lengthFraction) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  drawVortex(degreeOffset, lengthFraction);
  SDL_RenderPresent(renderer);
}

void Vortex::run() {
  int done;

  done = 0;
  float degreeOffset = 2.0f;
  float lengthFraction = 0.98;

  bool keyDown = false;
  while (!done) {
    SDL_Event event;
    drawScene(degreeOffset, lengthFraction);
    typedef decltype(event.type) lastEvent;

    int haveEvent = SDL_PollEvent(&event);
    // cout << "Event !!" << event.type << endl;
    if (event.type == SDL_KEYDOWN || keyDown) {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        done = 1;
        break;
      case SDLK_LEFT:
        degreeOffset += 0.005;
        break;
      case SDLK_RIGHT:
        degreeOffset -= 0.005;
        break;
      case SDLK_UP: {
        float newFract = lengthFraction + 0.0005;
        lengthFraction = newFract < 1.0 ? newFract : lengthFraction;
        break;
      }
      case SDLK_DOWN:
        if (event.key.keysym.sym == SDLK_DOWN) {
          lengthFraction -= 0.0005;
        };
        break;
      };
    }

    if (event.type == SDL_QUIT) {
      done = 1;
    }
  }
  SDL_Quit();
};

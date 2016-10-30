#include <SDL2/SDL.h>
#include <iostream>

// This guys tutorials -> http://headerphile.com/

struct Context {

  SDL_Renderer *renderer;
  SDL_Window *window;

  int x;
  int y;
};

void cb(void *p) {
  auto ctx = reinterpret_cast<Context *>(p);
  //std::cout << "TImer x=" << ctx->x << " y=" << ctx->y << "\n";
  SDL_SetRenderDrawColor(ctx->renderer, 0,0,0, 255);
  SDL_RenderClear(ctx->renderer);
  // SDL_RenderClear( ctx->renderer );
  SDL_Rect r{ ctx->x, ctx->y, 25, 25 };
  SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(ctx->renderer, &r);
  // Render the changes above ( which up until now had just happened behind the
  // scenes )
  ctx->x += 1;
  ctx->y += 1;

  // std::cout << SDL_GetThreadName() << std::endl;
  SDL_RenderPresent(ctx->renderer);
};

uint32_t cb2(uint32_t currentInterval, void *p) {
  SDL_Event event;
  SDL_UserEvent userevent;

  /* In this example, our callback pushes a function
  into the queue, and causes our callback to be called again at the
  same interval: */

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = (void *)&cb;
  userevent.data2 = p;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);
  return (currentInterval);
}

int main(int argc, char *args[]) {
  int posX = 100;
  int posY = 200;
  int sizeX = 800;
  int sizeY = 600;
  SDL_Window *window;
  SDL_Renderer *renderer;

  // Initialize SDL
  // ==========================================================
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    // Something failed, print error and exit.
    std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create and init the window
  // ==========================================================
  window = SDL_CreateWindow("Server", posX, posY, sizeX, sizeY, 0);

  if (window == nullptr) {
    std::cout << "Failed to create window : " << SDL_GetError();
    return -1;
  }

  // Create and init the renderer
  // ==========================================================
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr) {
    std::cout << "Failed to create renderer : " << SDL_GetError();
    return -1;
  }

  // Render something
  // ==========================================================

  // Set size of renderer to the same as window
  SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);

  // Set color of renderer to red
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

  // Clear the window and make it all red
  SDL_RenderClear(renderer);
  SDL_Rect myRect{ 50, 50, 25, 25 };

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderDrawRect(renderer, &myRect);

  // Render the changes above ( which up until now had just happened behind the
  // scenes )
  SDL_RenderPresent(renderer);

  // Pause program so that the window doesn't disappear at once.
  // This willpause for 4000 milliseconds which is the same as 4 seconds

  Context ctx = { renderer, window, 20, 20 };

  auto x = SDL_AddTimer(25, &cb2, &ctx);

  std::cout << "Timer id is " << x << "\n";
  // SDL_Delay( 4000 );

  // std::cout << SDL_GetThreadName(SDL_Thread * thread) SDL_Event event;

  SDL_Event event;
  bool done = false;
  while (!done) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN: {
          switch( event.key.keysym.sym) {
          case SDLK_ESCAPE:
              done = 1;
              break;
          }
          break;
      };
      case SDL_USEREVENT: {
          /* and now we can call the function we wanted to call in the timer but
           * couldn't because of the multithreading problems */
          auto p = (void (*)(void *))event.user.data1;
          p(event.user.data2);
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

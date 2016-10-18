// http://www.youtube.com/watch?v=FPA5-1kTRtI
#include <SDL.h>

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);

  auto window = SDL_CreateWindow("My Game Window",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, 600, 400,
                                 SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

  auto renderer = SDL_CreateRenderer(window, -1, 0);

  // SDL_Surface *screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE |
  // SDL_OPENGL);

  bool running = true;
  Uint32 start;
  SDL_Event event;

  while (running) {
  };
};

// http://www.youtube.com/watch?v=FPA5-1kTRtI
#include <SDL/SDL.h>

int main( int argc, char *argv[]) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE | SDL_OPENGL);

  bool running = true;
  Uint32 start;
  SDL_Event event;

  while( running ) {
  };
};

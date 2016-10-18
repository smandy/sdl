#include "SDL.h"

#define WINDOW_WIDTH 320
#define WINDOW_HEIGHT 240
#define WINDOW_DEPTH 8

int main(int argc, char *argv[]) {
    //SDL_Window *window;
     Uint8       *p;
     int         x = 10; //x coordinate of our pixel
     int         y = 20; //y coordinate of our pixel
     
     /* Initialize SDL */
     SDL_Init(SDL_INIT_VIDEO);
     
     /* Initialize the window / window */
     auto window = SDL_CreateWindow("My Game Window",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH ,
                               WINDOW_HEIGHT,
                               SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

     auto renderer = SDL_CreateRenderer(window, -1, 0);
     
     //screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
     /* Make p point to the place we want to draw the pixel */
     //p = (Uint8 *)screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
     
     /* Draw the pixel! */
     //*p=0xff;
     
     /* update the screen (aka double buffering) */
     SDL_RenderPresent(renderer);
     while(1);
}

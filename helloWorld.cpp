#include "SDL.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_DEPTH 8

int main(int argc, char *argv[]) {
     SDL_Surface *screen;
     Uint8       *p;
     int         x = 10; //x coordinate of our pixel
     int         y = 20; //y coordinate of our pixel
     
     /* Initialize SDL */
     SDL_Init(SDL_INIT_VIDEO);
     
     /* Initialize the screen / window */
     screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);
     
     /* Make p point to the place we want to draw the pixel */
     p = (Uint8 *)screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
     
     /* Draw the pixel! */
     *p=0xff;
     
     /* update the screen (aka double buffering) */
     SDL_Flip(screen);
     while(1);
}

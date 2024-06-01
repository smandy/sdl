// To compile with gcc:  (tested on Ubuntu 14.04 64bit):
//	 g++ sdl2_opengl.cpp -lSDL2 -lGL
// To compile with msvc: (tested on Windows 7 64bit)
//   cl sdl2_opengl.cpp /I C:\sdl2path\include /link C:\path\SDL2.lib
//   C:\path\SDL2main.lib /SUBSYSTEM:CONSOLE /NODEFAULTLIB:libcmtd.lib
//   opengl32.lib

#include <GL/gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <array>
#include <stdexcept>
#include <iostream>

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 500
#define WinHeight 500

int main(int ArgCount, char **Args) {

  u32 WindowFlags = SDL_WINDOW_OPENGL;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    throw std::runtime_error("SDL_GetError()");
  } else {
    std::cout << "Init okay" << std::endl;
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  
  SDL_Window *Window =
      SDL_CreateWindow("OpenGL Test", 0, 0, WinWidth, WinHeight, WindowFlags);
  assert(Window);
  SDL_GLContext Context = SDL_GL_CreateContext(Window);
  SDL_GL_MakeCurrent(Window, Context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  b32 Running = 1;
  b32 FullScreen = 0;
  while (Running) {
    SDL_Event Event;
    while (SDL_PollEvent(&Event)) {
      if (Event.type == SDL_KEYDOWN) {
        switch (Event.key.keysym.sym) {
        case SDLK_ESCAPE:
          Running = 0;
          break;
        case 'f':
          FullScreen = !FullScreen;
          if (FullScreen) {
            SDL_SetWindowFullscreen(Window, WindowFlags |
                                                SDL_WINDOW_FULLSCREEN_DESKTOP);
          } else {
            SDL_SetWindowFullscreen(Window, WindowFlags);
          }
          break;
        default:
          break;
        }
      } else if (Event.type == SDL_QUIT) {
        Running = 0;
      }
    }

    //glViewport(0, 0, WinWidth, WinHeight);
    //glClearColor(0.0f, 255.0f, 1.f, 1.f);
    //glClear(GL_COLOR_BUFFER_BIT);

    auto renderer = SDL_CreateRenderer(
        Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // std::array<SDL_Point, 5> sdl_points { {
    //         {100, 100}, {200, 100}, {200, 200}, {100, 200} } };
    std::array<SDL_Point, 2> sdl_points { {
            {0, 0}, {200, 100} }};

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderDrawLines(renderer, sdl_points.data(), sdl_points.size());
    SDL_RenderPresent(renderer);
    
    SDL_GL_SwapWindow(Window);
  }
  return 0;
}

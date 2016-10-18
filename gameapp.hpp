#pragma once

#include "SDL.h"

class GameApp {
    
    int width, height;

    SDL_Window* window;
    SDL_Renderer* renderer;
    
public:
    GameApp(int width, int height);

    ~GameApp();
    
    void InitSDL();
    
    void InitGL(int width, int height);

    void handleResize(int width, int height);

    void draw();
};

#pragma once

#include "SDL.h"

class GameApp {
    

    SDL_Window* window;
    
public:
    int width, height;
    
    static GameApp* instance;
    
    SDL_Renderer* renderer;
    
    GameApp(int width, int height);

    ~GameApp();
    
    void InitSDL();
    
    void InitGL();

    void handleResize(int width, int height);

    void draw();
};

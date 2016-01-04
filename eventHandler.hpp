#pragma once

#include "SDL.h"

class EventHandler {

protected:
    bool running;

public:
    EventHandler();

    void onEvent( SDL_Event *event);

    virtual ~EventHandler();

    virtual void onKeyDown( SDL_Event& evt) = 0;

    virtual void onKeyUp( SDL_Event& evt) = 0;
    
    virtual void onQuit() = 0;

    virtual void drawScene() = 0;

    void mainLoop();
};

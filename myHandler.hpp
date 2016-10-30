#pragma once

#include "eventHandler.hpp"
#include "SDL_ttf.h"

enum PlayerState {
    STATIONARY,
    MOVING_LEFT,
    MOVING_RIGHT
};

class MyHandler : public EventHandler {
    float degreeOffset;
    float lengthFraction;
    bool keyDown;
    PlayerState state;
    uint32_t x = 50;
    uint32_t y = 10;

    TTF_Font* font  = nullptr;
    
public:
    MyHandler( SDL_Renderer* _renderer,
               float _degreeOffset ,
               float _lengthFraction)  : EventHandler(_renderer),
                                         degreeOffset(_degreeOffset),
                                         lengthFraction(_lengthFraction) {}
    virtual void onKeyDown(SDL_Event& event);
    
    virtual void onKeyUp(SDL_Event& event);

    virtual void onQuit();

    virtual void drawScene();

    void drawVortex(float, float);

    virtual ~MyHandler();

    void drawPlayer();
};

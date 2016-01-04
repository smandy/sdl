#pragma once

#include "eventHandler.hpp"

class MyHandler : public EventHandler {
    float degreeOffset;
    float lengthFraction;
    bool keyDown;
    
public:
    MyHandler( float _degreeOffset ,
               float _lengthFraction )  : degreeOffset(_degreeOffset),
                                          lengthFraction(_lengthFraction) {}
    virtual void onKeyDown(SDL_Event& event);
    
    virtual void onKeyUp(SDL_Event& event);

    virtual void onQuit();

    virtual void drawScene();

    void drawVortex(float, float);

    virtual ~MyHandler();
};

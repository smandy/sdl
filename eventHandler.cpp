#include "eventHandler.hpp"
#include <iostream>
#include <vector>
#include <SDL.h>
//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

EventHandler::EventHandler(SDL_Renderer* _renderer) : renderer(_renderer), running(true) {
    std::cout << "EventHandler" << std::endl;
};

EventHandler::~EventHandler() {
    std::cout << "~EventHandler" << std::endl;
};


void EventHandler::mainLoop() {
    SDL_Event event;    
    while (running) {
        drawScene();
        typedef decltype( event.type) lastEvexnt;
        int haveEvent = SDL_PollEvent(&event);
        //cout << "Event !!" << event.type << endl;
        if (haveEvent) {
            if ( event.type == SDL_KEYDOWN) {
                onKeyDown( event);
            } else if (event.type == SDL_KEYUP) {
                onKeyUp(event);
            }
        }
        
        //std::vector<int> xs;
        //xs.pop_back();
        if ( event.type == SDL_QUIT ) {
            onQuit();
        }
    }
};


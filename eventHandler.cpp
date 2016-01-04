#include "eventHandler.hpp"
#include <iostream>

EventHandler::EventHandler() : running(true) {
    std::cout << "EventHandler" << std::endl;
};

EventHandler::~EventHandler() {
    std::cout << "~EventHandler" << std::endl;
};

void EventHandler::mainLoop() {
    while ( running ) {
        SDL_Event event;    
        drawScene( );
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
        
        if ( event.type == SDL_QUIT ) {
            onQuit();
        }
    }
};


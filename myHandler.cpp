#include "myHandler.hpp"

#include <iostream>
#include <stdlib.h> //Needed for "exit" function

#include "mousey.hpp"
//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include <SDL.h>
#include <SDL_render.h>

#include "ScopeUtils.h"

void MyHandler::drawScene(){
    //Clear information from last draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective

    // glBegin( GL_LINE_STRIP );
    // glVertex3f( -1.0f, -1.0f, -5.0f);
    // glVertex3f(  1.0f, -1.0f, -5.0f);
    // glVertex3f(  1.0f,  1.0f, -5.0f);
    // glVertex3f( -1.0f,  1.0f, -5.0f);
    // glVertex3f( -1.0f, -1.0f, -5.0f);
    // glEnd();

    glBegin( GL_LINE_STRIP );
    drawVortex(degreeOffset, lengthFraction);
    glEnd();
    drawPlayer();


  if (font==nullptr) {
    font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 14);
  }
  static SDL_Renderer *renderer;
  auto buf = "woot";

  static SDL_Color cyan  = { 0   , 255 , 255 };
  static SDL_Color red   = { 255 ,   0 ,   0 };
  static SDL_Color green = { 0   , 255 ,   0 };
  SDL_RenderClear(renderer);
  
  auto surf2 = TTF_RenderText_Solid(font, buf, cyan);
  OnScopeExit s(
      [&]() { SDL_FreeSurface(surf2); });

  auto texture = SDL_CreateTextureFromSurface(renderer, surf2);

  //scope(exit) SDL_FreeSurface( surf2);
  // myRect.x = cast(short)  0;
  // myRect.y = cast(short)  h;
  // myRect.w = cast(ushort) surf2.w;
  // myRect.h = cast(ushort) surf2.h;
  // h += surf2.h;
  // auto surface = TTF_RenderText_Solid(font, buf.ptr, cyan);
  // scope(exit) SDL_FreeSurface(surface);
  
  SDL_RenderPresent(renderer);
}

void MyHandler::drawPlayer() {
    glBegin( GL_LINE_STRIP );
    glVertex3f( x, y, -5.0);
    glVertex3f(  x + 10, y , -5.0);
    glVertex3f(  x + 10, y + 10, -5.0);
    glVertex3f(  x , y + 10, -5.0);
    glVertex3f(  x, y , -5.0);
    glEnd();
};


void MyHandler::onKeyDown( SDL_Event& evt) {
    std::cout << "OnKeyPress : <" << SDL_GetKeyName( evt.key.keysym.sym) << " " << evt.key.keysym.sym << ">" << std::endl;
    keyDown = true;
    if ( evt.key.keysym.sym == SDLK_LEFT ) {
        std::cout << "Doit " << std::endl;
        state = PlayerState::MOVING_LEFT;
    } else if  ( evt.key.keysym.sym == SDLK_RIGHT) {
        state = PlayerState::MOVING_RIGHT;
    };
};

void MyHandler::onKeyUp( SDL_Event& evt) {
    std::cout << "OnKeyUp : <" << SDL_GetKeyName( evt.key.keysym.sym) << ">" << std::endl;
    state = PlayerState::STATIONARY;
    keyDown = false;
};

void MyHandler::drawVortex(float degreeOffset, float lengthFraction) {
    float l = 2.0;
    Mousey m(-1.0f, -1.0f);
    while( l > 0.05 ) {
        glVertex3f( m.getX(), m.getY(), -3.0f);
        m.move( l );
        m.turn( Mousey::HALF_PI + degreeOffset * Mousey::DEGREE);
        l *= lengthFraction;
    };
};

MyHandler::~MyHandler() {
    std::cout << "~MyHandler" << std::endl;
};

void MyHandler::onQuit() {
    running = false;
};


#include "myHandler.hpp"

#include <iostream>
#include <stdlib.h> //Needed for "exit" function

#include "mousey.h"
// Include OpenGL header files, so that we can use OpenGL
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

#include <algorithm>
#include <sstream>
#include <vector>
#include "gameapp.hpp"
#include "mousey.h"

void MyHandler::drawScene() {
  // std::cout << "drawscene" << "\n";
  // Clear information from last draw

  if (false) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // Switch to the drawing perspective
    glLoadIdentity();           // Reset the drawing perspective
  }

  // glBegin( GL_LINE_STRIP );
  // glVertex3f( -1.0f, -1.0f, -5.0f);
  // glVertex3f(  1.0f, -1.0f, -5.0f);
  // glVertex3f(  1.0f,  1.0f, -5.0f);
  // glVertex3f( -1.0f,  1.0f, -5.0f);
  // glVertex3f( -1.0f, -1.0f, -5.0f);
  // glEnd();
  // glBegin( GL_LINE_STRIP );
  // drawVortex(degreeOffset, lengthFraction);
  // glEnd();
  // drawPlayer();
  if (font == nullptr) {
    font = TTF_OpenFont(
        "/usr/share/fonts/TTF/LiberationMono-Regular.ttf", 24);
    if (!font) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
      // handle error
    };
    std::cout << "Have set font " << font << std::endl;
  }

  static SDL_Color cyan = { 0, 255, 255 };
  static SDL_Color red = { 255, 0, 0 };
  static SDL_Color green = { 0, 255, 0 };

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  std::array<SDL_Point,2> lines;
  
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  for (int j = 40;j<500;j += 30) {
      lines[0].x = 10;
      lines[0].y = j;
      lines[1].x = 300;
      lines[1].y = j + 100;
      //std::cout << lines[0] << std::endl;
      SDL_RenderDrawLines(renderer, lines.data(), 2);
      // TODO - pimp up lavavej to do std::arrays?
      //print_line(std::cout, lines);
  }
  
  for (const auto &x : { 10, 300 }) {
    for (int y = 0; y < 600; y += 20) {
      std::ostringstream oss;
      oss << "Wayhey x=" << x << " y=" << y;
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_Color textColor = { 255, 255, 255, 255 };
      auto surf2 = TTF_RenderText_Solid(font, oss.str().c_str(), textColor);
      if (!surf2) {
        printf("TTF_RenderText: %s\n", TTF_GetError());
      }

      auto texture = SDL_CreateTextureFromSurface(renderer, surf2);
      if (!texture) {
        printf("SDL_CreateTexturefromsurface: %s\n", SDL_GetError());
      };
      auto h = surf2->h;
      auto w = surf2->w;
      // std::cout << "h=" << h << " w=" << w << "buf=" << buf << "\n";
      // std::cout << "App width=" << GameApp::instance->width << " h=" <<
      // GameApp::instance->height << std::endl;
      // std::cout << oss.str() << std::endl;
      // "\n";
      SDL_FreeSurface(surf2);
      SDL_Rect dest = { x, y, w, h };
      SDL_RenderCopy(renderer, texture, nullptr, &dest);
      SDL_DestroyTexture(texture);

      // scope(exit) SDL_FreeSurface( surf2);
      // myRect.x = cast(short)  0;
      // myRect.y = cast(short)  h;
      // myRect.w = cast(ushort) surf2.w;
      // myRect.h = cast(ushort) surf2.h;
      // h += surf2.h;
      // auto surface = TTF_RenderText_Solid(font, buf.ptr, cyan);
      // scope(exit) SDL_FreeSurface(surface);
    }
  }
  SDL_RenderPresent(renderer);
};

void MyHandler::drawPlayer() {
  glBegin(GL_LINE_STRIP);
  glVertex3f(x, y, -5.0);
  glVertex3f(x + 10, y, -5.0);
  glVertex3f(x + 10, y + 10, -5.0);
  glVertex3f(x, y + 10, -5.0);
  glVertex3f(x, y, -5.0);
  glEnd();
};

void MyHandler::onKeyDown(SDL_Event &evt) {
  std::cout << "OnKeyPress : <" << SDL_GetKeyName(evt.key.keysym.sym) << " "
            << evt.key.keysym.sym << ">" << std::endl;
  keyDown = true;
  if (evt.key.keysym.sym == SDLK_LEFT) {
    std::cout << "Doit " << std::endl;
    state = PlayerState::MOVING_LEFT;
  } else if (evt.key.keysym.sym == SDLK_RIGHT) {
    state = PlayerState::MOVING_RIGHT;
  };
};

void MyHandler::onKeyUp(SDL_Event &evt) {
  std::cout << "OnKeyUp : <" << SDL_GetKeyName(evt.key.keysym.sym) << ">"
            << std::endl;
  state = PlayerState::STATIONARY;
  keyDown = false;
};

void MyHandler::drawVortex(float degreeOffset, float lengthFraction) {
  float l = 2.0;
  Mousey m(-1.0f, -1.0f);
  while (l > 0.05) {
    glVertex3f(m.getX(), m.getY(), -3.0f);
    m.move(l);
    m.turn(Mousey::HALF_PI + degreeOffset * Mousey::DEGREE);
    l *= lengthFraction;
  };
};

MyHandler::~MyHandler() {
  std::cout << "~MyHandler" << std::endl;
};

void MyHandler::onQuit() {
  running = false;
};

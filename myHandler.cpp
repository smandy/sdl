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

void MyHandler::drawScene(){
    //Clear information from last draw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective

    glBegin( GL_LINE_STRIP );

    glVertex3f( -1.0f, -1.0f, -5.0f);
    glVertex3f(  1.0f, -1.0f, -5.0f);
    glVertex3f(  1.0f,  1.0f, -5.0f);
    glVertex3f( -1.0f,  1.0f, -5.0f);
    glVertex3f( -1.0f, -1.0f, -5.0f);
      
    glEnd();

    glBegin( GL_LINE_STRIP );
    drawVortex(degreeOffset, lengthFraction);
    glEnd();
    
    SDL_GL_SwapBuffers();
}

void MyHandler::onKeyDown( SDL_Event& evt) {
    std::cout << "OnKeyPress" << evt.key.keysym.unicode << std::endl;
    keyDown = true;
};

void MyHandler::onKeyUp( SDL_Event& evt) {
    std::cout << "OnKeyUp" << evt.key.keysym.unicode << std::endl;
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


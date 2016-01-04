#include "gameapp.hpp"

#include "SDL/SDL.h"
#include <GL/glut.h>
#include <iostream>

GameApp::GameApp(int _width,
                 int _height) : width(_width), height(_height) {
    InitSDL();
    InitGL(width, height);
};


GameApp::~GameApp() {
    std::cout << "~GameApp - calling SDL_QUIT" << std::endl;
    SDL_Quit();
};

void GameApp::InitSDL() {
    /* Initialize SDL for video output */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    /* Create a 640x480 OpenGL screen */
    if ( SDL_SetVideoMode(640, 480, 0, SDL_OPENGL) == NULL ) {
        fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
        SDL_Quit();
        exit(2);
    }

    /* Set the title bar in environments that support it */
    SDL_WM_SetCaption("Vortex", NULL);
};

void GameApp::InitGL(int Width, int Height) {
    glViewport(0, 0, Width, Height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);
}


void GameApp::handleResize( int width, int height) {
    //Tell OpenGL how to convert from coordinates to pixel values

    glViewport(0, 0, width, height);
     
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
     
    //Set the camera perspective
    glLoadIdentity();      //Reset the camera
    gluPerspective(45.0,                  //The camera angle
                   (double)width / (double)height, //The width-to-height ratio
                   1.0,             //The near z clipping coordinate
                   200.0);          //The far z clipping coordinate

};

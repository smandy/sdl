#include "gameapp.hpp"

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <GL/glut.h>
#include <iostream>

GameApp::GameApp(int _width,
                 int _height) : width(_width),
                                height(_height) {
    InitSDL();
    //InitGL(); - initting GL Seems to kill TTF!
    TTF_Init();
    instance = this;
};


GameApp::~GameApp() {
    std::cout << "~GameApp - calling SDL_QUIT" << std::endl;
    SDL_Quit();
};

void GameApp::InitSDL() {
    std::cout << "InitSDL" << "\n";
    /* Initialize SDL for video output */
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
     window = SDL_CreateWindow("My Game Window",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               width ,
                               height,
                               SDL_WINDOW_OPENGL);
     renderer = SDL_CreateRenderer(window, -1, 0);
     std::cout << "Renderere is " << renderer << "\n";
    /* Set the title bar in environments that support it */
    //SDL_WM_SetCaption("Vortex", NULL);
};

void GameApp::InitGL() {
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
    glMatrixMode(GL_MODELVIEW);
}


void GameApp::handleResize( int width, int height) {
    //Tell OpenGL how to convert from coordinates to pixel values

    this->width = width;
    this->height = height;
    
    glViewport(0, 0, width, height);
     
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
    //Set the camera perspective
    glLoadIdentity();      //Reset the camera
    gluPerspective(45.0,                  //The camera angle
                   (double)width / (double)height, //The width-to-height ratio
                   1.0,             //The near z clipping coordinate
                   200.0);          //The far z clipping coordinate

};


GameApp* GameApp::instance = nullptr;

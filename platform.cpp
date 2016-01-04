#include <iostream>
#include <stdlib.h> //Needed for "exit" function

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

#include "eventHandler.hpp"

#include "mousey.hpp"
#include "gameapp.hpp"
#include "myHandler.hpp"

using namespace std;

int main(int argc, char** argv) {
    int done;
  
    GameApp   app(640, 480);
    MyHandler eventHandler(2.0f, 0.98);
  
    eventHandler.mainLoop();
    
    return 0; //This line is never reached
}

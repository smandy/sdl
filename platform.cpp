#include <iostream>
#include <stdlib.h> //Needed for "exit" function

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

#include "eventHandler.hpp"

#include "mousey.h"
#include "gameapp.hpp"
#include "myHandler.hpp"

struct MyStruct {
  std::string s;
  int x;

  void doit() {
    std::cout << "Woot" << std::endl;
  };
};

using namespace std;

int main(int argc, char **argv) {
  int done;

  GameApp app(800, 600);
  MyHandler eventHandler(app.renderer, 2.0f, 0.98);
  std::cout << "Enter mainloop" << std::endl;
  eventHandler.mainLoop();
  std::cout << "Exit mainloop" << std::endl;
  return 0; // This line is never reached
}

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
#include <vector>

using namespace std;

void InitGL(int Width, int Height) // We call this right after our OpenGL window
                                   // is created.
{
  glViewport(0, 0, Width, Height);
  glClearColor(0.0f, 0.0f, 0.0f,
               0.0f);      // This Will Clear The Background Color To Black
  glClearDepth(1.0);       // Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);    // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST); // Enables Depth Testing
  glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // Reset The Projection Matrix

  gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f,
                 100.0f); // Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);
}

void handleResize(int w, int h) {
  // Tell OpenGL how to convert from coordinates to pixel values

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION); // Switch to setting the camera perspective

  // Set the camera perspective
  glLoadIdentity(); // Reset the camera
  gluPerspective(45.0, // The camera angle
                 (double)w / (double)h, // The width-to-height ratio
                 1.0, // The near z clipping coordinate
                 200.0); // The far z clipping coordinate
}

struct Mousey {
  static constexpr float PI = 4.0 * atan(1);
  static constexpr float HALF_PI = PI / 2.0;
  static constexpr float DEGREE = PI / 180.0;

  float direction;

  float x;
  float y;

  Mousey() : x(), y(), direction() {}
  Mousey(float x_, float y_, float direction_ = 0.0f)
      : x(x_), y(y_), direction(direction_) {}

  void move(float l) {
    x += l * cos(direction);
    y += l * sin(direction);
  };

  void turn(float angle) {
    direction += angle;
  };

  inline float getX() {
    return x;
  };
    
  inline float getY() {
    return y;
  };
};

void drawVortex(SDL_Renderer* renderer, float degreeOffset, float lengthFraction) {
  std::vector<SDL_Point> myVec;
  float l = 1000;
  Mousey m(0,0);
  while (l > 0.05) {
      myVec.push_back( { (int)m.getX(), (int)m.getY() });
      m.move(l);
      m.turn(Mousey::HALF_PI + degreeOffset * Mousey::DEGREE);
      l *= lengthFraction;
  };
  SDL_RenderDrawLines(renderer, myVec.data(), myVec.size());
};

void drawScene(SDL_Renderer *renderer, float degreeOffset, float lengthFraction) {
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor( renderer, 0, 255, 255, 255);
  drawVortex(renderer, degreeOffset, lengthFraction);
  SDL_RenderPresent(renderer);
}
int main(int argc, char **argv) {
  int done;

  /* Initialize SDL for video output */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  auto window = SDL_CreateWindow("Vortex", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, 1000, 1000,
                                 SDL_WINDOW_SHOWN);

  auto renderer = SDL_CreateRenderer(window,
                                     -1,
                                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  /* Set the title bar in environments that support it */
  // SDL_WM_SetCaption("Vortex", NULL);

  /* Loop, drawing and checking events */
  //  InitGL(640, 480);

  // drawScene();
  done = 0;
  float degreeOffset = 2.0f;
  float lengthFraction = 0.98;

  bool keyDown = false;
  while (!done) {
    SDL_Event event;
    drawScene(renderer, degreeOffset, lengthFraction);
    typedef decltype(event.type) lastEvent;

    int haveEvent = SDL_PollEvent(&event);
    // cout << "Event !!" << event.type << endl;
    if (event.type == SDL_KEYDOWN || keyDown) {
        switch( event.key.keysym.sym) {
        case SDLK_ESCAPE:
            done = 1;
            break;
        case SDLK_LEFT:
            degreeOffset += 0.005;
            break;
        case SDLK_RIGHT:
            degreeOffset -= 0.005;
            break;
        case SDLK_UP: {
            float newFract = lengthFraction + 0.0005;
            lengthFraction = newFract < 1.0 ? newFract : lengthFraction;
            break;
        }
        case SDLK_DOWN:
            if (event.key.keysym.sym == SDLK_DOWN) {
                lengthFraction -= 0.0005;
            };
            break;
        };
    }

    if (event.type == SDL_QUIT) {
      done = 1;
    }

    //drawScene(renderer,degreeOffset, lengthFraction);
  }
  SDL_Quit();
  return 0; // This line is never reached
}

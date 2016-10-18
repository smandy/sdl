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
  float l = 2.0;
  Mousey m(-1.0f, -1.0f);
  while (l > 0.05) {
    glVertex3f(m.getX(), m.getY(), -3.0f);
    m.move(l);
    m.turn(Mousey::HALF_PI + degreeOffset * Mousey::DEGREE);
    l *= lengthFraction;
  };
};

void drawScene(SDL_Renderer *renderer, float degreeOffset, float lengthFraction) {
  // Clear information from last draw
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW); // Switch to the drawing perspective
  glLoadIdentity(); // Reset the drawing perspective

  glBegin(GL_LINE_STRIP);

  glVertex3f(-1.0f, -1.0f, -5.0f);
  glVertex3f(1.0f, -1.0f, -5.0f);
  glVertex3f(1.0f, 1.0f, -5.0f);
  glVertex3f(-1.0f, 1.0f, -5.0f);
  glVertex3f(-1.0f, -1.0f, -5.0f);

  glEnd();

  glBegin(GL_LINE_STRIP);
  drawVortex(renderer,degreeOffset, lengthFraction);
  glEnd();

  // cout << "Drawing scene " << endl;

  //SDL_GL_SwapBuffers(); // Send the 3D scene to the screen
  SDL_RenderPresent(renderer);
}
int main(int argc, char **argv) {
  int done;

  /* Initialize SDL for video output */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  auto window = SDL_CreateWindow("My Game Window", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, 600, 400,
                                 SDL_WINDOW_OPENGL);

  auto renderer = SDL_CreateRenderer(window, -1, 0);

  /* Set the title bar in environments that support it */
  // SDL_WM_SetCaption("Vortex", NULL);

  /* Loop, drawing and checking events */
  InitGL(640, 480);

  // drawScene();
  done = 0;
  float degreeOffset = 2.0f;
  float lengthFraction = 0.98;

  bool keyDown = false;
  while (!done) {
    SDL_Event event;
    
    drawScene(renderer, degreeOffset, lengthFraction);
    typedef decltype(event.type) lastEvexnt;

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
        }
            break;
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

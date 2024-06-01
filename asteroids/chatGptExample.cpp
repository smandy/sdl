#include <SDL.h>
#include <SDL_opengl.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Rectangle position
float rectX = SCREEN_WIDTH / 2.0f;
float rectY = SCREEN_HEIGHT / 2.0f;
const float rectWidth = 50.0f;
const float rectHeight = 50.0f;
const float speed = 5.0f;

void handleInput(SDL_Event &e, bool &quit) {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
        // User requests quit
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            // Adjust the rectangle's position based on the key press
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    rectY -= speed;
                    break;
                case SDLK_DOWN:
                    rectY += speed;
                    break;
                case SDLK_LEFT:
                    rectX -= speed;
                    break;
                case SDLK_RIGHT:
                    rectX += speed;
                    break;
            }
        }
    }
}

void render() {
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Set rectangle color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw the rectangle
    glBegin(GL_QUADS);
        glVertex2f(rectX, rectY);
        glVertex2f(rectX + rectWidth, rectY);
        glVertex2f(rectX + rectWidth, rectY + rectHeight);
        glVertex2f(rectX, rectY + rectHeight);
    glEnd();
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL2 OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize OpenGL
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle input
        handleInput(e, quit);

        // Render
        render();

        // Update screen
        SDL_GL_SwapWindow(window);
    }

    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}

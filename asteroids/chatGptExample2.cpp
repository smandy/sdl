// Opengl free sdl2


#include <SDL.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Rectangle position and size
SDL_Rect rect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50 };
const int speed = 5;

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
                    rect.y -= speed;
                    break;
                case SDLK_DOWN:
                    rect.y += speed;
                    break;
                case SDLK_LEFT:
                    rect.x -= speed;
                    break;
                case SDLK_RIGHT:
                    rect.x += speed;
                    break;
            }
        }
    }
}

void render(SDL_Renderer* renderer) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set rectangle color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw the rectangle
    SDL_RenderFillRect(renderer, &rect);

    // Update screen
    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }


    int numRenderDrivers = SDL_GetNumRenderDrivers();
    printf("Number of render drivers available: %d\n", numRenderDrivers);

    // Print the names of the available rendering drivers
    for (int i = 0; i < numRenderDrivers; ++i) {
        SDL_RendererInfo info;
        if (SDL_GetRenderDriverInfo(i, &info) == 0) {
            printf("Render driver %d: %s\n", i, info.name);
        }
    }

    
    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Rectangle Movement", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create renderer for window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    } else {
        SDL_RendererInfo info;
        if (SDL_GetRendererInfo(renderer, &info) == 0) {
            printf("Currently using renderer: %s\n", info.name);
        }
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle input
        handleInput(e, quit);

        // Render
        render(renderer);
    }

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}

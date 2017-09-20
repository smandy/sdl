#include "game.h"

#include "Box2D/Box2D.h"
#include <SDL.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>


uint32_t my_timer_func(uint32_t interval, void *ctx) {
  // std::cout << "My Timer" << std::endl;
  SDL_Event event;
  SDL_UserEvent userevent;
  /* In this example, our callback pushes an SDL_USEREVENT event
     into the queue, and causes our callback to be called again at the
     same interval: */
  userevent.type = SDL_USEREVENT;
  userevent.code = 1;
  userevent.data1 = ctx;
  userevent.data2 = (void *)(size_t)interval;
  event.type = SDL_USEREVENT;
  event.user = userevent;
  SDL_PushEvent(&event);
  return interval;
}

Game::Game() : game_running{true}, running{true} {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }
  window =
      SDL_CreateWindow("B2d", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       1200, 800, SDL_WINDOW_SHOWN);

  if (!window) {
    std::cout << "Error creating window " << SDL_GetError() << std::endl;
    exit(1);
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer) {
    std::cout << "Error creating renderer " << SDL_GetError() << std::endl;
    exit(1);
  }
  uint32_t delay = 400; /* To round it down to the nearest 10 ms */
  SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);
}

void Game::draw() {
  static SDL_Rect r;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void Game::on_timer(uint32_t interval) {}

void Game::run() {
  running = true;

  // Define the gravity vector.
  b2Vec2 gravity(0.0f, -10.0f);

  // Construct a world object, which will hold and simulate the rigid bodies.
  b2World world(gravity);

  //MyContactListener myContactListenerInstance;
  
  //in FooTest constructor
  //m_world->SetContactListener(&myContactListenerInstance);

  // Define the ground body.
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);

  // Call the body factory which allocates memory for the ground body
  // from a pool and creates the ground box shape (also from a pool).
  // The body is also added to the world.
  b2Body *groundBody = world.CreateBody(&groundBodyDef);

  // Define the ground box shape.
  b2PolygonShape groundBox;

  // The extents are the half-widths of the box.
  groundBox.SetAsBox(50.0f, 10.0f);

  // Add the ground fixture to the ground body.
  groundBody->CreateFixture(&groundBox, 0.0f);

  // Define the dynamic body. We set its position and call the body factory.
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);
  b2Body *body = world.CreateBody(&bodyDef);

  // Define another box shape for our dynamic body.
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  // Define the dynamic body fixture.
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;

  // Set the box density to be non-zero, so it will be dynamic.
  fixtureDef.density = 1.0f;

  // Override the default friction.
  fixtureDef.friction = 0.3f;

  // Add the shape to the body.
  body->CreateFixture(&fixtureDef);

  // Prepare for simulation. Typically we use a time step of 1/60 of a
  // second (60Hz) and 10 iterations. This provides a high quality simulation
  // in most game scenarios.
  float32 timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  int i = 0;

  uint32_t time = SDL_GetTicks();
  while (running) {
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.

    auto newt = SDL_GetTicks();
    auto dt = newt - time;
    time = newt;
    world.Step(dt / 1000.0, velocityIterations, positionIterations);

    // Now print the position and angle of the body.
    b2Vec2 position = body->GetPosition();
    float32 angle = body->GetAngle();

    // printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Rect r;
    r.x = 400;
    r.y = 800 - 200 * position.y;
    r.w = 30;
    r.h = 30;
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);

    draw();
    process_input_events();
    i++;
  }
  SDL_Quit();
}


void Game::process_input_events() {
  SDL_Event event;
  uint32_t lastEvent;
  bool keyDown = false;
  int haveEvent = SDL_PollEvent(&event);
  if (haveEvent) {

    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {

      case SDLK_r: {
        break;
      }
      case SDLK_ESCAPE: {
        running = false;
        break;
      }
      }
    }

    if (event.type == SDL_KEYDOWN && game_running) {
      switch (event.key.keysym.sym) {
      case SDLK_SPACE: {
        break;
      }
      case SDLK_p: {
        break;
      }
      case SDLK_a: {
        break;
      }
      case SDLK_h: {
        break;
      }
      case SDLK_n: {
        break;
      }
      case SDLK_RIGHT: {
        break;
      }
      case SDLK_LEFT: {
        break;
      }
      case SDLK_UP: {
        break;
      }
      case SDLK_DOWN:
        break;
      }
    }

    if (event.type == SDL_QUIT) {
      running = false;
    }

    if (event.type == SDL_USEREVENT) {
      reinterpret_cast<Game *>(event.user.data1)
          ->on_timer(reinterpret_cast<size_t>(event.user.data2));
    }
  }
}

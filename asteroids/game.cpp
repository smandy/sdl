#include "game.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

#include "GL/gl3w.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <SDL2/SDL.h>

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

Game::~Game() { ImGui_ImplSdlGL2_Shutdown(); };

Game::Game() : game_running{true}, running{true}, gui{false}, show_ctrl{false} {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }

  // SDL_Window *window = SDL_CreateWindow("ImGui SDL2+OpenGL3 example",
  // SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
  // SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (!window) {
    std::cout << "Error creating window " << SDL_GetError() << std::endl;
    exit(1);
  }
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);

  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  gl3wInit();
  ImGui_ImplSdlGL2_Init(window);

  // std::cout << "Woot" << std::endl;
  if (!renderer) {
    std::cout << "Error creating renderer " << SDL_GetError() << std::endl;
    exit(1);
  }
  uint32_t delay = 400; /* To round it down to the nearest 10 ms */
  SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);
}

void Game::on_timer(uint32_t interval) {}

void Game::run() {
  running = true;
  while (running) {
    process_input_events();

    if (game_running) {
      f.update_state();
    }

    f.draw(renderer);
    // SDL_RenderPresent(renderer);
    ImGui_ImplSdlGL2_NewFrame(window);

    if (gui) {
      ImGui::ShowDemoWindow();
    }

    if (show_ctrl) {
        //ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
        //ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
      ImGui::Begin("Asteroid controls");
      ImGui::Text("Have a play.");

      ImGui::SliderFloat("ship scale", &Constants::SHIP_SCALE, 0.2f, 1.0f,
                         "ship scale = %.3f");
      ImGui::SliderFloat("asteroid scale", &Constants::ASTEROID_SCALE, 0.2f,
                         1.0f, "asteroid scale = %.3f");
      ImGui::SliderInt("bullet size", &Constants::BULLET_WIDTH, 1, 20, nullptr);
      ImGui::SliderInt("theta incr", &Constants::THETA_INCR, 1, 10, nullptr);
      ImGui::End();
    }

    glUseProgram(0);
    ImGui::Render();
    SDL_RenderPresent(renderer);
  }
  SDL_Quit();
}

void Game::process_input_events() {
  SDL_Event event;
  uint32_t lastEvent;
  bool keyDown = false;
  int haveEvent = SDL_PollEvent(&event);

  auto keys = SDL_GetKeyboardState(nullptr);

  if (keys[SDL_SCANCODE_LEFT]) {
    f.theta -= Constants::THETA_INCR * DEGREE_TO_RADIAN;
  }

  if (keys[SDL_SCANCODE_RIGHT]) {
    f.theta += Constants::THETA_INCR * DEGREE_TO_RADIAN;
  }

  if (keys[SDL_SCANCODE_LCTRL]) {
    auto dv = std::polar(0.2f, f.theta);
    f.entities[SHIP_ID].velocity += dv;
    // std::cout << "dv is " << dv << std::endl;
    // std::cout << "Velocity now " << f.entities[SHIP_ID].velocity <<
    // std::endl;
  }

  if (haveEvent) {
    if (gui) {
      ImGui_ImplSdlGL2_ProcessEvent(&event);
    }
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_r: {
        break;
      }
      case SDLK_h: {
        gui = !gui;
        std::cout << "WOot2" << (gui ? "true" : "false") << std::endl;
        break;
      }
      case SDLK_w: {
        show_ctrl = !show_ctrl;
        break;
      }
      case SDLK_p: {
        game_running = !game_running;
        break;
      }
      case SDLK_ESCAPE: {
        running = false;
        break;
      }
      }
    }
    if (event.type == SDL_KEYUP) {
      // std::cout << "WOot - key up" << std::endl;
    }

    if (event.type == SDL_KEYDOWN && game_running) {
      switch (event.key.keysym.sym) {
      case SDLK_RETURN: {
        // std::cout << "Fire bullet" << std::endl;
        f.fire_bullet();
        break;
      }
      case SDLK_SPACE: {
        break;
      }
      case SDLK_p: {
        break;
      }
      case SDLK_a: {
        break;
      }
      case SDLK_n: {
        break;
      }
      case SDLK_RIGHT: {
        // f.theta += 5 * DEGREE_TO_RADIAN;
        break;
      }
      case SDLK_LCTRL: {
      }

      case SDLK_LEFT: {
        // f.theta -= 5 * DEGREE_TO_RADIAN;
      }
      case SDLK_UP: {
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

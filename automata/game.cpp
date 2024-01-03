#include "game.h"

#include <SDL.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

#include "GL/gl3w.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "types.h"
#include <mutex>

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

Game::Game() : game_running{true}, running{true}, gui_visible{false} {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }
  window =
      SDL_CreateWindow("Automata", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN);

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

  uint32_t delay = 400; /* To round it down to the nearest 10 ms */
  SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);
}

void Game::draw() {
  static SDL_Rect r;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  // static std::once_flag my_flag;
  //  std::call_once(my_flag, [=]() {
  if (gui_visible) {
    ImGui_ImplSdlGL2_NewFrame(window);
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::Begin("Automata controls");
    // ImGui::Text("Have a play.");
    // ImGui::SliderFloat("ship scale", &Constants::SHIP_SCALE, 0.2f, 1.0f,
    //                    "ship scale = %.3f");
    // ImGui::SliderFloat("asteroid scale", &Constants::ASTEROID_SCALE,
    // 0.2f, 1.0f,
    //                    "asteroid scale = %.3f");
    ImGui::SliderInt("rule", &automata.rule, 1, 256, "%d");
    // ImGui::SliderInt("", &Constants::THETA_INCR, 1, 10, nullptr);
    ImGui::End();
    glUseProgram(0);
    ImGui::Render();
  };
  SDL_RenderPresent(renderer);
}

void Game::on_timer(uint32_t interval) {}

void Game::run() {
  running = true;
  while (running) {
    draw();
    process_input_events();
  }
  SDL_Quit();
}

void Game::process_input_events() {
  SDL_Event event;
  uint32_t lastEvent;
  bool keyDown = false;
  int haveEvent = SDL_PollEvent(&event);
  if (haveEvent) {

    ImGui_ImplSdlGL2_ProcessEvent(&event);

    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_r: {
        break;
      }
      case SDLK_w: {
        gui_visible = !gui_visible;
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

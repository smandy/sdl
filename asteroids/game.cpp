#include "game.h"

#include <iostream>

// #include "GL/gl3w.h"
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"
#include "types.h"
#include <SDL.h>
#include "GL/gl3w.h"
#include <complex>
#include <cstdint>
#include <format>
#include <stdexcept>

// At initialization:
//   call ImGui::CreateContext()
//   call ImGui_ImplXXXX_Init() for each backend.
//
// At the beginning of your frame:
//   call ImGui_ImplXXXX_NewFrame() for each backend.
//   call ImGui::NewFrame()
//
// At the end of your frame:
//   call ImGui::Render()
//   call ImGui_ImplXXXX_RenderDrawData() for your Renderer backend.
//
// At shutdown:
//   call ImGui_ImplXXXX_Shutdown() for each backend.
//   call ImGui::DestroyContext()

auto my_timer_func(uint32_t interval, void *ctx) -> uint32_t {
  // std::cout << "My Timer" << std::endl;
  SDL_Event event;
  SDL_UserEvent userevent;

  /* In this example, our callback pushes an SDL_USEREVENT event
     into the queue, and causes our callback to be called again at the
     same interval: */
  userevent.type = SDL_USEREVENT;
  userevent.code = 1;
  userevent.data1 = ctx;

  event.type = SDL_USEREVENT;
  event.user = userevent;
  SDL_PushEvent(&event);
  return interval;
}

Game::Game()
    : game_running{true}, running{true}, gui{false}, show_ctrl{false},
      clear_color{0.0, 0.0, 0.60, 1.00} {
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    std::cout << std::format("Error: {}\n", SDL_GetError());
    throw std::runtime_error(std::format("SDL_GetError() {}", SDL_GetError()));
  }
  std::cout << "Init okay\n";

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  auto windowFlags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlags);
  if (window == nullptr) {
    std::cout << std::format("Error: SDL_CreateWindow(): {}\n", SDL_GetError());
    throw std::runtime_error(SDL_GetError());
  }
  std::cout << "Window create okay\n";

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  glContext = SDL_GL_CreateContext(window);

  SDL_GL_MakeCurrent(window, glContext);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Setup Dear ImGui contextrende
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL2_Init();

  ImGuiIO &io2 = ImGui::GetIO();
  //(void)io;
  io2.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io2.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  // ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Our state
  // bool show_demo_window = true;
  // bool show_another_window = false;

  std::cout << "Game ctor exiting\n";
}

// uint32_t delay = 400; /* To round it down to the nearest 10 ms */
// SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);

// Main loop
//  run();
// Cleanup
Game::~Game() {
  std::cout << "Game dtor running\n";
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "Game dtor exiting\n";
}

void Game::run() {
  // ImVec4 clear_color2 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  while (running) {
    // std::cout << "Loop" << std::endl;
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      // std::cout << " event!" << std::endl;
      processInputEvents(event);
      ImGui_ImplSDL2_ProcessEvent(&event);
      // process_input_events(event);
      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window)) {
        running = false;
      }
    }

    SDL_RenderClear(renderer);
    const ImGuiIO &io2 = ImGui::GetIO();
    glViewport(0, 0, (int)io2.DisplaySize.x, (int)io2.DisplaySize.y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    //  SDL_RenderPresent(renderer);
    //  ImGui_ImplSDL2_NewFrame();
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (game_running) {
      f.update_state();
    }

    if (gui) {
      ImGui::ShowDemoWindow();
    }

    maybeShowControls();
    f.draw(renderer);

    ImGui::Render();

    // glUseProgram(0);

    //      ImGuiIO &io = ImGui::GetIO();
    //      (void)io;
    //      io.ConfigFlags |=
    //          ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //      io.ConfigFlags |=
    //          ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // std::cout << " io is " << io << std::endl;

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
    SDL_GL_SwapWindow(window);
  }
};

void Game::maybeShowControls() const {
  if (show_ctrl) {
    // ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Asteroid controls");
    ImGui::Text("Have a play.");

    ImGui::SliderFloat("ship scale", &Constants::SHIP_SCALE, 0.2, 1.0,
                       "ship scale = %.3f");
    ImGui::SliderFloat("asteroid scale", &Constants::ASTEROID_SCALE, 0.2, 1.0,
                       "asteroid scale = %.3f");
    ImGui::SliderInt("bullet size", &Constants::BULLET_WIDTH, 1, 20, nullptr);
    ImGui::SliderFloat("theta incr", &Constants::THETA_INCR, 1, 10, nullptr);
    ImGui::End();
  }
};

void Game::onTimer() {}

void Game::processInputEvents(SDL_Event &event) {
  // bool keyDown = false;
  // int haveEvent = SDL_PollEvent(&event);
  const auto *keys = SDL_GetKeyboardState(nullptr);
  if (bool(keys[SDL_SCANCODE_LEFT])) {
    f.theta -= Constants::THETA_INCR * DEGREE_TO_RADIAN;
  }

  if (bool(keys[SDL_SCANCODE_RIGHT])) {
    f.theta += Constants::THETA_INCR * DEGREE_TO_RADIAN;
  }

  if (bool(keys[SDL_SCANCODE_LCTRL])) {
    auto deltaVee = std::polar(0.2f, f.theta);
    f.entities[SHIP_ID].velocity += deltaVee;
  }

  if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
    case SDLK_r: {
      break;
    }
    case SDLK_h: {
      gui = !gui;
      std::cout << "WOot2" << (gui ? "true\n" : "false\n");
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
    default: {
        // Nothing
    }
    }
  }
  if (event.type == SDL_KEYUP) {
      std::cout << "WOot - key up\n";
  }

  if (event.type == SDL_KEYDOWN && game_running && event.key.keysym.sym==SDLK_RETURN) {
      f.fire_bullet();
  }

  if (event.type == SDL_QUIT) {
    running = false;
  }

  if (event.type == SDL_USEREVENT) {
    static_cast<Game *>(event.user.data1)->onTimer();
  }
}

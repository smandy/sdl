#include "game.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

// #include "GL/gl3w.h"
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"
#include <SDL.h>
#include <SDL_opengl.h>

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

Game::Game()
    : game_running{true}, running{true}, gui{false}, show_ctrl{false},
      clear_color{0.0f, 0.0f, 0.60f, 1.00f} {
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    throw std::runtime_error("SDL_GetError()");
  } else {
    std::cout << "Init okay" << std::endl;
  }

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
  SDL_WindowFlags window_flags = 
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    throw std::runtime_error(SDL_GetError());
  } else {
    std::cout << "Window create okay" << std::endl;
  };

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  gl_context = SDL_GL_CreateContext(window);
  
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Setup Dear ImGui contextrende
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io2 = ImGui::GetIO();
  //(void)io;
  io2.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io2.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL2_Init();

  // Our state
  // bool show_demo_window = true;
  // bool show_another_window = false;

  std::cout << "Game ctor exiting" << std::endl;
}

// uint32_t delay = 400; /* To round it down to the nearest 10 ms */
// SDL_TimerID my_timer_id = SDL_AddTimer(delay, my_timer_func, (void *)this);

// Main loop
//  run();
// Cleanup
Game::~Game() {
  std::cout << "Game dtor running" << std::endl;
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  std::cout << "Game dtor exiting" << std::endl;
}

void Game::run() {
    //ImVec4 clear_color2 = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  while (running) {
    // std::cout << "Loop" << std::endl;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // std::cout << " event!" << std::endl;
      ImGui_ImplSDL2_ProcessEvent(&event);
      process_input_events(event);
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

    // SDL_RenderPresent(renderer);
    // ImGui_ImplSDL2_NewFrame();
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    if (game_running) {
        f.update_state();
    }

    if (gui || true) {
      ImGui::ShowDemoWindow();
    }
    ImGui::Render();

    if (show_ctrl && false) {
      // ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
      // ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
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

    // glUseProgram(0);

    //      ImGuiIO &io = ImGui::GetIO();
    //      (void)io;
    //      io.ConfigFlags |=
    //          ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //      io.ConfigFlags |=
    //          ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // std::cout << " io is " << io << std::endl;
    ImGuiIO &io2 = ImGui::GetIO();
    glViewport(0, 0, (int)io2.DisplaySize.x, (int)io2.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    f.draw(renderer);
    SDL_RenderPresent(renderer);
    SDL_GL_SwapWindow(window);
  }
};

void Game::maybe_show_controls() {
  if (show_ctrl) {
    // ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Asteroid controls");
    ImGui::Text("Have a play.");

    ImGui::SliderFloat("ship scale", &Constants::SHIP_SCALE, 0.2f, 1.0f,
                       "ship scale = %.3f");
    ImGui::SliderFloat("asteroid scale", &Constants::ASTEROID_SCALE, 0.2f, 1.0f,
                       "asteroid scale = %.3f");
    ImGui::SliderInt("bullet size", &Constants::BULLET_WIDTH, 1, 20, nullptr);
    ImGui::SliderInt("theta incr", &Constants::THETA_INCR, 1, 10, nullptr);
    ImGui::End();
  }
};

void Game::on_timer(uint32_t interval) {}

void Game::process_input_events(SDL_Event &event) {
  // bool keyDown = false;
  // int haveEvent = SDL_PollEvent(&event);

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

  // // if (gui) {
  // //   ImGui_ImplSDL2_ProcessEvent(&event);
  //}
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
    std::cout << "WOot - key up" << std::endl;
  }

  if (event.type == SDL_KEYDOWN && game_running) {
    switch (event.key.keysym.sym) {
    case SDLK_RETURN: {
      // std::cout << "Fire bullet" << std::endl;
      f.fire_bullet();
      break;
    }
    case SDLK_SPACE | SDLK_p | SDLK_a | SDLK_n | SDLK_LCTRL : {
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

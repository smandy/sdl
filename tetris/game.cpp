#include "game.h"

#include <SDL.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>

// Our coordinate system
// row major.
// row[0][6] center of bottom cell

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
  return (interval);
}

void Game::init_tetr() {
  game_running = false;
  std::vector<uint8_t> perms(Tetronimo::tetronimos.size());
  std::iota(std::begin(perms), std::end(perms), 0);
  std::random_shuffle(std::begin(perms), std::end(perms));

  while (!perms.empty() && !game_running) {
    tetr = perms.back();
    perms.pop_back();
    std::vector<uint8_t> rots(4);
    std::iota(std::begin(rots), std::end(rots), 0);
    std::random_shuffle(rots.begin(), rots.end());
    while (!rots.empty() && !game_running) {

      rot = rots.back();
      rots.pop_back();

      std::vector<uint8_t> xs(Well::WIDTH);
      std::iota(std::begin(xs), std::end(xs), 0);

      std::random_shuffle(std::begin(xs), std::end(xs));
      while (!xs.empty() && !game_running) {
        tx = xs.back();
        xs.pop_back();
        for (uint8_t i = 2; i < 4; ++i) {
          ty = Well::HEIGHT - i;
          if (can_move_to(t(), ty, tx)) {
            game_running = true;
            break;
          }
        }
      }
    }
  }
}

const Tetronimo &Game::t() { return Tetronimo::tetronimos[tetr][rot]; };

Game::Game()
    : t_idx{}, offset_x{6}, offset_y{4}, show_periodic_table{false},
      antigravity(false), game_running{true} {
  init_tetr();
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    std::cout << "Error initializing sdl " << SDL_GetError() << std::endl;
    exit(1);
  }
  window =
      SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       BLOCK_SIZE * Well::WIDTH + 100,
                       BLOCK_SIZE * Well::HEIGHT + 150, SDL_WINDOW_SHOWN);

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

void Game::draw(const Tetronimo &t, int y, int x, bool outline) {
  for (int bx = 0; bx < 4; ++bx) {
    for (int by = 0; by < 4; ++by) {
      if (t._data[by][bx]) {
        draw_block(y + by, x + bx, outline);
      }
    }
  }
}

void Game::on_timer(uint32_t interval) {
  if (!antigravity && game_running) {
    down_impl();
  }
}

void Game::draw() {
  static SDL_Rect r;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
  for (int x = 0; x < Well::WIDTH + 1; x++) {
    for (int y = -1; y < Well::HEIGHT; y++) {
      set_block_ul(y, x, r);
      SDL_RenderDrawLine(renderer, r.x - 3, r.y, r.x + 3, r.y);
      SDL_RenderDrawLine(renderer, r.x, r.y - 3, r.x, r.y + 3);
    }
  }

  if (show_periodic_table == true) {
    int x = offset_x;
    int y = offset_y;
    for (int j = 0; j < Tetronimo::tetronimos.size(); ++j) {
      x = offset_x;
      set_tetr_color(j);
      for (int i = 0; i < 4; ++i) {
        auto t = Tetronimo::tetronimos[j][i];
        draw(t, y, x, false);
        x += 5;
      }
      y += 5;
    }
  }

  draw_well();
  if (game_running) {
    set_tetr_color(tetr);
    draw(t(), ty, tx, false);

    int8_t best_y = ty;
    while (true) {
      auto y = best_y - 1;
      if (can_move_to(t(), y, tx)) {
        best_y = y;
      } else {
        break;
      }
    }
    draw(t(), best_y, tx, true);
  }
  SDL_RenderPresent(renderer);
}

void Game::draw_well() {
  for (int8_t y = 0; y < Well::HEIGHT; ++y) {
    for (int8_t x = 0; x < Well::WIDTH; ++x) {
      if (w.rows[y][x] != -1) {
        set_tetr_color(w.rows[y][x]);
        draw_block(y, x, false);
      }
    }
  }
}

void Game::set_block_ul(int8_t y, int8_t x, SDL_Rect &r) {
  r.x = x * BLOCK_SIZE + 50;
  r.y = (Well::HEIGHT - y + 1) * BLOCK_SIZE + 50;
}

void Game::set_tetr_color(uint32_t i) {
  SDL_Color &c = colors[i];
  SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void Game::draw_block(int y, int x, bool outline) {
  if (outline) {
    draw_block_transparent(y, x);
  } else {
    draw_block_filled(y, x);
  }
}

void Game::draw_block_transparent(int y, int x) {
  SDL_Rect r;
  set_block_ul(y, x, r);

  static std::array<uint8_t, 3> x1s{0, 3, 4};
  static std::array<uint8_t, 3> y1s{0, 1, 4};
  static std::array<uint8_t, 2> x2s{1, 2};
  static std::array<uint8_t, 2> y2s{2, 3};
  apply_to(x1s, [&](SDL_Point &p) { p.x = r.x; });
  apply_to(y1s, [&](SDL_Point &p) { p.y = r.y; });
  apply_to(x2s, [&](SDL_Point &p) { p.x = r.x + BLOCK_SIZE - 2; });
  apply_to(y2s, [&](SDL_Point &p) { p.y = r.y + BLOCK_SIZE - 2; });
  SDL_RenderDrawLines(renderer, points.data(), 5);
}

void Game::draw_block_filled(int y, int x) {
  static SDL_Rect r;
  set_block_ul(y, x, r);
  r.w = BLOCK_SIZE - 2;
  r.h = BLOCK_SIZE - 2;
  SDL_RenderFillRect(renderer, &r);
}

bool Game::can_move_to(const Tetronimo &z, uint8_t y, uint8_t x) {
  auto ret = w.in_well(z, y, x) && !w.collides(z, y, x);
  return ret;
};

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
  typedef decltype(event.type) lastEvent;
  bool keyDown = false;
  int haveEvent = SDL_PollEvent(&event);
  if (haveEvent) {
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_r: {
        game_running = true;
        w.reset();
        init_tetr();
        break;
      }
      case SDLK_ESCAPE: {
        running = false break;
      }
      }
    }

    if (event.type == SDL_KEYDOWN && game_running) {
      switch (event.key.keysym.sym) {
      case SDLK_SPACE: {
        while (down_impl()) {
        };
        break;
      }
      case SDLK_p: {
        show_periodic_table = !show_periodic_table;
        break;
      }
      case SDLK_a: {
        antigravity = !antigravity;
        break;
      }
      case SDLK_n: {
        init_tetr();
        break;
      }
      case SDLK_RIGHT: {
        if (show_periodic_table)
          ++offset_x;
        if (can_move_to(t(), ty, tx + 1)) {
          tx += 1;
        }
        break;
      }
      case SDLK_LEFT: {
        if (show_periodic_table)
          --offset_x;
        if (can_move_to(t(), ty, tx - 1)) {
          tx -= 1;
        }
        break;
      }
      case SDLK_UP: {
        if (show_periodic_table)
          ++offset_y;
        if (antigravity) {
          if (can_move_to(t(), ty + 1, tx)) {
            ty += 1;
          }
        } else {
          auto new_rot = (rot + 1) % 4;
          const Tetronimo &new_tet = Tetronimo::tetronimos[tetr][new_rot];
          if (can_move_to(new_tet, ty, tx)) {
            rot = new_rot;
            // std::cout << " new rot is " << new_rot << std::endl;
          }
        }
        break;
      }
      case SDLK_DOWN:
        if (show_periodic_table)
          --offset_y;
        down_impl();
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

bool Game::check_rows() {
  int y = 0;
  while (y < Well::HEIGHT) {
    if (std::all_of(std::begin(w.rows[y]), std::end(w.rows[y]),
                    [](auto x) { return x != -1; })) {
      for (auto i = y; i < Well::HEIGHT - 1; ++i)
        w.rows[i] = w.rows[i + 1];
      w.rows[Well::HEIGHT - 1] = w.empty_row;
      continue;
    }
    y++;
  }
}

bool Game::down_impl() {
  if (can_move_to(t(), ty - 1, tx)) {
    ty -= 1;
  } else {
    w.place(t(), ty, tx, tetr);
    check_rows();
    init_tetr();
  }
}

// clang-format off
std::vector<SDL_Color> Game::colors = {
    {  0, 255, 255, 0},
    {  0,   0, 255, 0},
    {255, 165,   0, 0},
    {255, 255,   0, 0},
    {  0, 255, 255, 0},
    {255,   0, 255, 0},
    {255,   0,   0, 0}};
// clang-format on

std::array<SDL_Point, 5> Game::points = {};

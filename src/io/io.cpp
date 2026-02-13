#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "io.hpp"

IO::IO() {
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw SDLError{SDLError::Type::SDL_INIT_ERROR, SDL_GetError()};

  w_ = SDL_CreateWindow("CHIP8", 64, 32, 0);
  if (!w_)
    throw SDLError{SDLError::Type::SDL_WINDOW_ERROR, SDL_GetError()};

  r_ = SDL_CreateRenderer(w_, nullptr);
  if (!r_)
    throw SDLError{SDLError::Type::SDL_RENDERER_ERROR, SDL_GetError()};
}

IO::~IO() {
  SDL_DestroyRenderer(r_);
  SDL_DestroyWindow(w_);
  SDL_Quit();
}
void IO::eventLoop() const {
  bool running = true;
  SDL_Event event;

  // -- START Event loop
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(r_, 0, 0, 0, 255);
    SDL_RenderClear(r_);
    SDL_RenderPresent(r_);
  }
  // -- END Event Loop
};

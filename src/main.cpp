#include "SDL3/SDL_init.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <iostream>
#include <print>

namespace {

enum Error {
    SDL_INIT_ERROR = 0,
    SDL_WINDOW_ERROR,
    SDL_RENDERER_ERROR
};

} // anonymous namespace

int main(int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      std::println(std::cerr, "Failed to initialize SDL3. Error: {}",
                   SDL_GetError());
      return SDL_INIT_ERROR;
    }
    std::atexit(SDL_Quit);

    SDL_Window *window = SDL_CreateWindow("CHIP8", 64, 32, 0);
    if (!window) {
      std::println(std::cerr, "Failed to create window. Error: {}",
                   SDL_GetError());
      return SDL_WINDOW_ERROR;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
      std::println(std::cerr, "Failed to create renderer. Error: {}",
                   SDL_GetError());
      return SDL_RENDERER_ERROR;
    }

    bool running = true;
    SDL_Event event;

    // -- START Event loop
    while (running) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
          running = false;
        }
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }
    // -- END Event Loop

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

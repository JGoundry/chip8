#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "io.hpp"

IO::IO() {
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw SDLError{SDLError::Type::SDL_INIT_ERROR, SDL_GetError()};

  w_ = SDL_CreateWindow("CHIP8", 1280, 640, 0);
  if (!w_)
    throw SDLError{SDLError::Type::SDL_WINDOW_ERROR, SDL_GetError()};

  r_ = SDL_CreateRenderer(w_, nullptr);
  if (!r_)
    throw SDLError{SDLError::Type::SDL_RENDERER_ERROR, SDL_GetError()};
  SDL_SetRenderLogicalPresentation(r_, 64, 32,
                                   SDL_LOGICAL_PRESENTATION_STRETCH);

  t_ = SDL_CreateTexture(r_, SDL_PIXELFORMAT_ARGB8888,
                         SDL_TEXTUREACCESS_STREAMING, 64, 32);
  if (!t_)
    throw SDLError{SDLError::Type::SDL_TEXTURE_ERROR, SDL_GetError()};
}

IO::~IO() {
  SDL_DestroyTexture(t_);
  SDL_DestroyRenderer(r_);
  SDL_DestroyWindow(w_);
  SDL_Quit();
}

void IO::pollEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
      break;
    case SDL_EVENT_KEY_UP:
      break;
    case SDL_EVENT_QUIT:
      quit_ = true;
    default:
      break;
    }
  }
};

void IO::render(const VideoBuf &buf) {
  SDL_UpdateTexture(t_, nullptr, buf.data(), 64 * sizeof(unsigned char));
  SDL_RenderClear(r_);
  SDL_RenderTexture(r_, t_, nullptr, nullptr);
  SDL_RenderPresent(r_);
}

const IO::Keys &IO::getKeys() const { return k_; }

bool IO::quitSignal() const { return quit_; }

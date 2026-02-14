#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>

#include <cstdint>

#include "SDL3/SDL_surface.h"
#include "io.hpp"

namespace {

constexpr auto bufWidth{64};
constexpr auto bufHeight{32};

constexpr auto windowWidth{1280};
constexpr auto windowHeight{640};

} // namespace

IO::IO() {
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw SDLError{SDLError::Type::SDL_INIT_ERROR, SDL_GetError()};

  w_ = SDL_CreateWindow("CHIP8", windowWidth, windowHeight, 0);
  if (!w_)
    throw SDLError{SDLError::Type::SDL_WINDOW_ERROR, SDL_GetError()};

  r_ = SDL_CreateRenderer(w_, nullptr);
  if (!r_)
    throw SDLError{SDLError::Type::SDL_RENDERER_ERROR, SDL_GetError()};
  SDL_SetRenderLogicalPresentation(r_, bufWidth, bufHeight,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  t_ = SDL_CreateTexture(r_, SDL_PIXELFORMAT_ARGB8888,
                         SDL_TEXTUREACCESS_STREAMING, bufWidth, bufHeight);
  if (!t_)
    throw SDLError{SDLError::Type::SDL_TEXTURE_ERROR, SDL_GetError()};
  SDL_SetTextureScaleMode(t_, SDL_SCALEMODE_PIXELART);
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
    case SDL_EVENT_QUIT:
      quit_ = true;
      break;
    default:
      break;
    }
  }
};

void IO::render(const VideoBuf &buf) {
  SDL_UpdateTexture(t_, nullptr, buf.data(), bufWidth * sizeof(uint32_t));
  SDL_RenderClear(r_);
  SDL_RenderTexture(r_, t_, nullptr, nullptr);
  SDL_RenderPresent(r_);
}

IO::Keys IO::getKeys() const {
  SDL_PumpEvents();

  int numKeys;
  const bool *const keyboard = SDL_GetKeyboardState(&numKeys);

  // Keyboard:
  // 1 2 3 4
  // Q W E R
  // A S D F
  // Z X C V

  return {
      keyboard[SDL_SCANCODE_1], keyboard[SDL_SCANCODE_2],
      keyboard[SDL_SCANCODE_3], keyboard[SDL_SCANCODE_4],
      keyboard[SDL_SCANCODE_Q], keyboard[SDL_SCANCODE_W],
      keyboard[SDL_SCANCODE_E], keyboard[SDL_SCANCODE_R],
      keyboard[SDL_SCANCODE_A], keyboard[SDL_SCANCODE_S],
      keyboard[SDL_SCANCODE_D], keyboard[SDL_SCANCODE_F],
      keyboard[SDL_SCANCODE_Z], keyboard[SDL_SCANCODE_X],
      keyboard[SDL_SCANCODE_C], keyboard[SDL_SCANCODE_V],
  };
}

bool IO::quitSignal() const noexcept { return quit_; }

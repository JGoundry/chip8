#pragma once

#include <array>
#include <cstdint>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

struct SDLError {
  enum Type {
    SDL_INIT_ERROR = 0,
    SDL_WINDOW_ERROR,
    SDL_RENDERER_ERROR,
    SDL_TEXTURE_ERROR
  };
  const Type t;
  const std::string msg;
};

class IO final {
public:
  using Keys = std::array<bool, 16>;
  using VideoBuf = std::array<uint8_t, 64 * 32>;

  IO();
  ~IO();

  IO(const IO &) = delete;
  IO &operator=(const IO &) = delete;
  IO(IO &&) = delete;
  IO &operator=(IO &&) = delete;

  void pollEvents();
  void render(const VideoBuf &buf);
  bool quitSignal() const;
  const Keys &getKeys() const;

private:
  SDL_Window *w_;
  SDL_Renderer *r_;
  SDL_Texture *t_;
  Keys k_;
  bool quit_{};
};

#pragma once

#include <array>
#include <cstdint>
#include <string>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

struct IOError {
  enum Type {
    INIT_ERROR = 0
  };
  const Type t;
  const std::string msg;
};

class IO final {
public:
  using Keys = std::array<bool, 16>;
  using VideoBuf = std::array<uint32_t, 64 * 32>;

  IO();
  ~IO();

  IO(const IO &) = delete;
  IO &operator=(const IO &) = delete;
  IO(IO &&) = delete;
  IO &operator=(IO &&) = delete;

  void pollEvents() noexcept;
  void render(const VideoBuf& buf) noexcept;
  bool quitSignal() const noexcept;
  Keys getKeys() const noexcept;

private:
  SDL_Window *w_;
  SDL_Renderer *r_;
  SDL_Texture *t_;
  bool quit_{};
};

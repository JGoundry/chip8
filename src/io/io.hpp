#pragma once

#include <string>

struct SDL_Window;
struct SDL_Renderer;

struct SDLError {
  enum Type { SDL_INIT_ERROR = 0, SDL_WINDOW_ERROR, SDL_RENDERER_ERROR };
  const Type t;
  const std::string msg;
};

class IO final {
public:
  IO();
  ~IO();

  IO(const IO &) = delete;
  IO &operator=(const IO &) = delete;
  IO(IO &&) = delete;
  IO &operator=(IO &&) = delete;

  void eventLoop() const;

private:
  SDL_Window *w_;
  SDL_Renderer *r_;
};

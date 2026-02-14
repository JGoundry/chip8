#pragma once

#include "keys.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "video.hpp"

#include <array>
#include <cstdint>
#include <filesystem>

class Chip8 final {
public:
  [[nodiscard]] bool loadROM(const std::filesystem::path &filepath);
  void cycle() noexcept;
  void setKeys(const Keys &k) noexcept;
  const VideoBuf &videoBuf() const noexcept;

private:
  /*
   *  Helpers
   */
  void incPC() noexcept;
  void decPC() noexcept;
  uint8_t h() const noexcept;
  uint8_t n() const noexcept;
  uint8_t x() const noexcept;
  uint8_t y() const noexcept;
  uint8_t kk() const noexcept;
  uint16_t nnn() const noexcept;

  /*
   *  Instruction handlers
   */
  void handle0() noexcept;
  void handle1() noexcept;
  void handle2() noexcept;
  void handle3() noexcept;
  void handle4() noexcept;
  void handle5() noexcept;
  void handle6() noexcept;
  void handle7() noexcept;
  void handle8() noexcept;
  void handle9() noexcept;
  void handleA() noexcept;
  void handleB() noexcept;
  void handleC() noexcept;
  void handleD() noexcept;
  void handleE() noexcept;
  void handleF() noexcept;

private:
  using HandlerTable = std::array<void (Chip8::*)() noexcept, 16>;

  uint16_t instruction_{};
  Memory m_{bootMemory()};
  Stack s_{};
  Registers r_{.PC = StartAddress};
  VideoBuf v_{};
  Keys k_{};
  HandlerTable handlerTable_{
      &Chip8::handle0, &Chip8::handle1, &Chip8::handle2, &Chip8::handle3,
      &Chip8::handle4, &Chip8::handle5, &Chip8::handle6, &Chip8::handle7,
      &Chip8::handle8, &Chip8::handle9, &Chip8::handleA, &Chip8::handleB,
      &Chip8::handleC, &Chip8::handleD, &Chip8::handleE, &Chip8::handleF,
  };
};

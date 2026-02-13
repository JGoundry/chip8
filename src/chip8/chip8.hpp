#pragma once

#include "memory.hpp"
#include "registers.hpp"
#include "video.hpp"

#include <array>
#include <cstdint>
#include <filesystem>

class Chip8 final {
public:
  void loadROM(const std::filesystem::path &filepath);
  void cycle();

private:
  void handle0();
  void handle1();
  void handle2();
  void handle3();
  void handle4();
  void handle5();
  void handle6();
  void handle7();
  void handle8();
  void handle9();
  void handleA();
  void handleB();
  void handleC();
  void handleD();
  void handleE();
  void handleF();

private:
  using HandlerTable = std::array<void (Chip8::*)(), 16>;

  uint16_t instruction_;
  Memory m_{bootMemory()};
  Stack s_{};
  Registers r_{.PC = StartAddress};
  VideoBuf v_{};
  HandlerTable handlerTable_{
      &Chip8::handle0, &Chip8::handle1, &Chip8::handle2, &Chip8::handle3,
      &Chip8::handle4, &Chip8::handle5, &Chip8::handle6, &Chip8::handle7,
      &Chip8::handle8, &Chip8::handle9, &Chip8::handleA, &Chip8::handleB,
      &Chip8::handleC, &Chip8::handleD, &Chip8::handleE, &Chip8::handleF,
  };
};

#pragma once

#include "memory.hpp"
#include "registers.hpp"
#include "video.hpp"

class Chip8 final {
  Memory m_{bootMemory()};
  Stack s_{};
  Registers r_{.PC = StartAddress};
  VideoBuf v_{};
  // keypad array?

public:
  void loadROM();
  void cycle();

private:
  // 36 instruction functions:
  void op_00E0() { // cls
    v_.fill(0);
  }
  // 00EE ret
};

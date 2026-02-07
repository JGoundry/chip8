#pragma once

#include "memory.hpp"
#include "registers.hpp"

class Chip8 final {
  Memory    m_;
  Stack     s_;
  Registers r_;
};

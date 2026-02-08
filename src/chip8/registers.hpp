#pragma once

#include <array>
#include <cstdint>

/*
 *  16 GPR registers (8-bit). V0 to VF. VF reserved for some instructions.
 *  ----------------------------------------------------------------------
 *  I  (16-bit):  Generally used to store memory addresses (12 LSB used)
 *  PC (16-bit):  Program counter
 *  SP (8-bit):   Stack pointer
 *  DT (8-bit):   Delay timer
 *  ST (8-bit):   Sound timer (sound)
 */

enum GeneralPurposeRegister : std::uint8_t {
  V0 = 0,
  V1,
  V2,
  V3,
  V4,
  V5,
  V6,
  V7,
  V8,
  V9,
  VA,
  VB,
  VC,
  VD,
  VE,
  VF,
};

using GPR = std::array<std::uint8_t, 16>;

struct Registers {
  std::uint16_t I;
  std::uint16_t PC;
  std::uint8_t SP;
  std::uint8_t DT;
  std::uint8_t ST;
  GPR gpr;
};

#include "chip8.hpp"

namespace {

constexpr uint8_t getLastNibble(uint16_t i) { return (i & 0x000F); }

} // anonymous namespace

/*
 *  Load ROM from disk into memory starting at 0x200
 */
void Chip8::loadROM(const std::filesystem::path& filepath) {

}

/*
 *  Decrement ST and DT if non-zero (should be at 60Hz rate, whatever...)
 *  Fetch 2 byte instruction (big endian) from PC into instruction_
 *  Decode first nibble (4 bits) to get instruction category
 *  Dispatch to handler for category
 */
void Chip8::cycle() {
  --r_.ST;
  --r_.DT;
  instruction_ = (m_[r_.PC] << 8) | m_[r_.PC + 1];
  const uint8_t firstNibble = (instruction_ & 0xF000) >> 12;
  (this->*handlerTable_[firstNibble])();
}

/*
 *  Instruction handlers
 */
void Chip8::handle0() {
  // 0nnn jump to nnn
  // 00E0 cls
  // 00EE return
}
void Chip8::handle1() {
  // 1nnn JP addr
}
void Chip8::handle2() {
  // 2nnn CALL addr
}
void Chip8::handle3() {
  // 3xkk SE Vx, byte - Skip next instruction if Vx == kk
}
void Chip8::handle4() {
  // 4xkk SNE Vx, byte - Skip next instruction if Vx != kk
}
void Chip8::handle5() {
  // 5xy0 SE Vx, Vy - Skip next instruction if Vx == Vy
}
void Chip8::handle6() {
  // 6xkk
}
void Chip8::handle7() {
  // 7xkk
}
void Chip8::handle8() {
  const uint8_t lastNibble = getLastNibble(instruction_);
  // 8xy0
  // 8xy1
  // 8xy2
  // 8xy3
  // 8xy4
  // 8xy5
  // 8xy6
  // 8xy7
  // 8xyE
}
void Chip8::handle9() {
  // 9xy0
}
void Chip8::handleA() {
  // Annn
};
void Chip8::handleB() {
  // Bnnn
};
void Chip8::handleC() {
  // Cxkk
};
void Chip8::handleD() {
  // Dxyn
};
void Chip8::handleE() {
  // Ex9E
  // ExA1
};
void Chip8::handleF() {
  // Fx07
  // Fx0A
  // Fx15
  // Fx18
  // Fx1E
  // Fx29
  // Fx33
  // Fx55
  // Fx65
};

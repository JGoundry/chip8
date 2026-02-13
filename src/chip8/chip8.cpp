#include "chip8.hpp"
#include "memory.hpp"

#include <cstdint>
#include <fstream>

/*
 *  Load ROM from disk into memory starting at 0x200
 */
bool Chip8::loadROM(const std::filesystem::path &filepath) {
  if (!std::filesystem::exists(filepath))
    return false;

  std::ifstream file(filepath, std::ios_base::in | std::ios_base::binary);
  if (!file.is_open())
    return false;

  file.read(reinterpret_cast<char *>(m_.data() + StartAddress),
            m_.size() - StartAddress);

  return true;
}

/*
 *  Decrement ST and DT if non-zero (should be at 60Hz rate, whatever...)
 *  Fetch 2 byte instruction (big endian) from PC into instruction_
 *  Decode first nibble (4 bits) to get instruction category
 *  Dispatch to handler for category
 */
void Chip8::cycle() {
  if (r_.ST)
    --r_.ST;
  if (r_.DT)
    --r_.DT;

  instruction_ = (m_[r_.PC] << 8) | m_[r_.PC + 1];
  incPC();
  const uint8_t firstNibble = (instruction_ & 0xF000) >> 12;
  (this->*handlerTable_[firstNibble])();
}

void Chip8::setKeys(const Keys &k) { k_ = k; }
const VideoBuf &Chip8::videoBuf() const { return v_; }

/*
 *  Helpers
 */
void Chip8::incPC() { r_.PC += 2; }
void Chip8::decPC() { r_.PC -= 2; }
uint8_t Chip8::n() const {
  // n or nibble - lowest 4 bits of instruction
  return (0x000F & instruction_);
}
uint8_t Chip8::x() const {
  // x - lower 4 bits of MSB
  return (0x0F00 & instruction_) >> 8;
}
uint8_t Chip8::y() const {
  // y - upper 4 bits of LSB
  return (0x00F0 & instruction_) >> 4;
}
uint8_t Chip8::kk() const {
  // kk or byte - LSB
  return (0x00FF & instruction_);
}
uint16_t Chip8::nnn() const {
  // kkk - lowest 12 bits
  return (0x0FFF & instruction_);
}

/*
 *  Instruction handlers
 */
void Chip8::handle0() {
  switch (instruction_) {
  case 0x00E0: // cls
    v_.fill(0);
    break;
  case 0x00EE: // ret
    r_.PC = s_[r_.SP--];
    break;
  default: // 0nnn (ignored)
    break;
  }
}
void Chip8::handle1() {
  // JP addr
  r_.PC = nnn();
}
void Chip8::handle2() {
  // CALL addr
  s_[++r_.SP] = r_.PC;
  handle1();
}
void Chip8::handle3() {
  // 3xkk SE Vx, byte - Skip next instruction if Vx == kk
  if (r_.gpr[x()] == kk())
    incPC();
}
void Chip8::handle4() {
  // 4xkk SNE Vx, byte - Skip next instruction if Vx != kk
  if (r_.gpr[x()] != kk())
    incPC();
}
void Chip8::handle5() {
  // 5xy0 SE Vx, Vy - Skip next instruction if Vx == Vy
  if (r_.gpr[x()] == r_.gpr[y()])
    incPC();
}
void Chip8::handle6() {
  // 6xkk - LD Vx, byte
  r_.gpr[x()] = kk();
}
void Chip8::handle7() {
  // 7xkk - ADD Vx, byte
  r_.gpr[x()] += kk();
}
void Chip8::handle8() {
  switch (n()) {
  case 0x0: // 8xy0 - LD Vx, Vy
    r_.gpr[x()] = r_.gpr[y()];
    break;
  case 0x1: // 8xy1 - OR Vx, Vy
    break;
  case 0x2: // 8xy2 - AND Vx, Vy
    break;
  case 0x3: // 8xy3 - XOR Vx, Vy
    break;
  case 0x4: // 8xy4 - ADD Vx, Vy
    break;
  case 0x5: // 8xy5 - SUB Vx, Vy
    break;
  case 0x6: // 8xy6 - SHR Vx {, Vy}
    break;
  case 0x7: // 8xy7 - SUBN Vx, Vy
    break;
  case 0xE: // 8xyE - SHL Vx {, Vy}
    break;
  default:
    break;
  }
}
void Chip8::handle9() {
  // 9xy0 - SNE Vx, Vy - Skip next instruction if Vx != Vy
  if (r_.gpr[x()] != r_.gpr[y()])
    incPC();
}
void Chip8::handleA() {
  // Annn - LD I, adrr
  r_.I = nnn();
};
void Chip8::handleB() {
  // Bnnn - JP V0, addr - jump to nnn + V0
  r_.PC = r_.gpr[V0] + nnn();
};
void Chip8::handleC() {
  // Cxkk - RND Vx, byte
  r_.gpr[x()] = 0; // TODO: not random
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

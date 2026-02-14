#include "chip8.hpp"
#include "memory.hpp"

#include <cstdint>
#include <fstream>

// temp
#include <print>

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
  (this->*handlerTable_[h()])();
}

void Chip8::setKeys(const Keys &k) noexcept { k_ = k; }
const VideoBuf &Chip8::videoBuf() const noexcept { return v_; }

/*
 *  Helpers
 */
void Chip8::incPC() noexcept { r_.PC += 2; }
void Chip8::decPC() noexcept { r_.PC -= 2; }
uint8_t Chip8::h() const noexcept {
  // h - highest 4 bits of instruction
  return (0xF000 & instruction_) >> 12;
}
uint8_t Chip8::n() const noexcept {
  // n or nibble - lowest 4 bits of instruction
  return (0x000F & instruction_);
}
uint8_t Chip8::x() const noexcept {
  // x - lower 4 bits of MSB
  return (0x0F00 & instruction_) >> 8;
}
uint8_t Chip8::y() const noexcept {
  // y - upper 4 bits of LSB
  return (0x00F0 & instruction_) >> 4;
}
uint8_t Chip8::kk() const noexcept {
  // kk or byte - LSB
  return (0x00FF & instruction_);
}
uint16_t Chip8::nnn() const noexcept {
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
    r_.PC = s_[--r_.SP];
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
  s_[r_.SP++] = r_.PC;
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
    r_.gpr[x()] |= r_.gpr[y()];
    break;
  case 0x2: // 8xy2 - AND Vx, Vy
    r_.gpr[x()] &= r_.gpr[y()];
    break;
  case 0x3: // 8xy3 - XOR Vx, Vy
    r_.gpr[x()] ^= r_.gpr[y()];
    break;
  case 0x4: { // 8xy4 - ADD Vx, Vy, set VF = carry
    const uint16_t result = r_.gpr[x()] + r_.gpr[y()];
    r_.gpr[VF] = result > 0x00FF;
    r_.gpr[x()] = (result & 0x00FF);
    break;
  }
  case 0x5: { // 8xy5 - SUB Vx, Vy, set VF = NOT borrow
    const uint8_t Vx = x();
    const uint8_t Vy = y();
    r_.gpr[VF] = r_.gpr[Vx] > r_.gpr[Vy];
    r_.gpr[Vx] -= r_.gpr[Vy];
    break;
  }
  case 0x6: { // 8xy6 - SHR Vx {, Vy} - ignoring Vy
    const uint8_t Vx = x();
    r_.gpr[VF] = 0x01 & r_.gpr[Vx];
    r_.gpr[Vx] >>= 1;
    break;
  }
  case 0x7: { // 8xy7 - SUBN Vx, Vy
    const uint8_t Vx = x();
    const uint8_t Vy = y();
    r_.gpr[VF] = r_.gpr[Vy] > r_.gpr[Vx];
    r_.gpr[Vx] = r_.gpr[Vy] - r_.gpr[Vx];
    break;
  }
  case 0xE: { // 8xyE - SHL Vx {, Vy} - ignoring Xy
    const uint8_t Vx = x();
    r_.gpr[VF] = 0x80 & r_.gpr[Vx];
    r_.gpr[Vx] <<= 1;
    break;
  }
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
  std::println("Cxkk");
};
void Chip8::handleD() {
  // Dxyn - DRW Vx, Vy, nibble
  // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF =
  // collision

  r_.gpr[VF] = 0; // reset collision flag

  const uint8_t nByte = n();
  const uint8_t col = r_.gpr[x()];
  const uint8_t row = r_.gpr[y()];

  for (uint8_t byteOffset{}; byteOffset < nByte; ++byteOffset) {
    const uint8_t byte = m_[r_.I + byteOffset];

    for (uint8_t bitOffset{}; bitOffset < 8; ++bitOffset) {
      if (byte & (0x80 >> bitOffset)) {
        const size_t vCol = (col + bitOffset) % 64; // modulo to wrap around
        const size_t vRow = (row + byteOffset) % 32;
        const size_t idx = vRow * 64 + vCol;

        // Set carry flag if we have a collision
        if (v_[idx])
          r_.gpr[VF] = 1;

        v_[idx] ^= 0xFFFFFFFF;
      }
    }
  }
};
void Chip8::handleE() {
  switch (kk()) {
  case 0x9E: // Ex9E - SKP Vx - Skip next instruction if key Vx pressed
    if (k_[x()])
      incPC();
    break;
  case 0xA1: // ExA1 - SKNP Vx - Skip next instruction if key Vx NOT pressed
    if (!k_[x()])
      incPC();
    break;
  default:
    break;
  }
};
void Chip8::handleF() {
  switch (kk()) {
  case 0x07: // Fx07 - LD Vx, DT
    r_.gpr[x()] = r_.DT;
    break;
  case 0x0A: // Fx0A - LD Vx, K - Wait for key press, store value of key in Vx
    std::println("Fx0A");
    break;
  case 0x15: // Fx15 - LD DT, Vx
    r_.DT = r_.gpr[x()];
    break;
  case 0x18: // Fx18 - LD ST, Vx
    r_.ST = r_.gpr[x()];
    break;
  case 0x1E: // Fx1E - ADD I, Vx
    r_.I += r_.gpr[x()];
    break;
  case 0x29: // Fx29 - LD F, Vx
    r_.I = FontsetStartAddress + r_.gpr[x()] * 5;
    break;
  case 0x33: { // Fx33 - LD B, Vx
    const uint8_t Vx = r_.gpr[x()];
    m_[r_.I] = Vx / 100;
    m_[r_.I + 1] = Vx % 100 / 10;
    m_[r_.I + 2] = Vx % 10;
    break;
  }
  case 0x55: { // Fx55 - [I], Vx
    const uint8_t n = x();
    for (uint8_t i{}; i <= n; ++i)
      m_[r_.I + i] = r_.gpr[i];
    break;
  }
  case 0x65: { // Fx65 - LD Vx, [I]
    const uint8_t n = x();
    for (uint8_t i{}; i <= n; ++i)
      r_.gpr[i] = m_[r_.I + i];
    break;
  }
  default:
    break;
  }
};

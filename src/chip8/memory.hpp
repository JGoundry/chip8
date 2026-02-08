#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <algorithm>

constexpr size_t StackSize = 16;
constexpr size_t MemorySize = 4096;
constexpr size_t FontsetSize = 80;

constexpr std::ptrdiff_t FontsetStartAddress = 0x50;
constexpr std::ptrdiff_t StartAddress = 0x200;

/*
 *  The stack is an array of 16 16-bit values, used to store the return address
 *  for a subroutine. Therefore, Chip-8 allows up to 16 levels of nested subroutines.
 */

using Stack = std::array<std::uint16_t, StackSize>;

/*
 *  Chip-8 has 4KB (4096 bytes) of RAM. 0x000 to 0xFFF.
 *  0x000 to 0x1FF are where the original interpreter lived.
 *
 *  +---------------+= 0xFFF (4095) End of Chip-8 RAM
 *  |               |
 *  |               |
 *  |               |
 *  |               |
 *  |               |
 *  | 0x200 to 0xFFF|
 *  |     Chip-8    |
 *  | Program / Data|
 *  |     Space     |
 *  |               |
 *  |               |
 *  |               |
 *  +- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
 *  |               |
 *  |               |
 *  |               |
 *  +---------------+= 0x200 (512) Start of most Chip-8 programs
 *  | 0x000 to 0x1FF|
 *  | Reserved for  |
 *  |  interpreter  |
 *  +---------------+= 0x000 (0) Start of Chip-8 RAM
 *
 *  Source: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1
 */

using Memory = std::array<std::uint8_t, MemorySize>;

/*
 *  Chip-8 has a built in font for the hex values
 *  Each char is 8x5 in size
 */

constexpr std::array<std::uint8_t, FontsetSize> Fontset{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


/*
 *  Fontset goes into memory reserved for interpreter at 0x50 to 0xA0
 */
constexpr Memory bootMemory() {
    Memory m{};
    std::ranges::copy(Fontset, (m.data() + FontsetStartAddress));
    return m;
}

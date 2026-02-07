#pragma once

#include <array>
#include <cstdint>

/*
 *  The stack is an array of 16 16-bit values, used to store the return address
 *  for a subroutine. Therefore, Chip-8 allows up to 16 levels of nested subroutines.
 */

using Stack = std::array<std::uint16_t, 16>;

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

using Memory = std::array<std::uint8_t, 4096>;

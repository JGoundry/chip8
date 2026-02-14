#pragma once

#include <cstdint>
#include <array>

/*
 *  Chip-8 displays a 64x32 screen.
 */

using VideoBuf = std::array<uint32_t, 64*32>;

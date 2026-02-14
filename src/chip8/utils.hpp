#pragma once

#include <iosfwd>

#include "memory.hpp"
#include "video.hpp"

void printMemory(std::ostream& os, const Memory &m);
void printVideoBuf(std::ostream& os, const VideoBuf &m);

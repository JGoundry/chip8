#include <iostream>
#include <iomanip>

#include "utils.hpp"
#include "memory.hpp"

void printMemory(std::ostream& os, const Memory &m) {
  for (size_t i{StartAddress}; i < m.size(); ++i) {
    os << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(m[i]) << ' ';
  }
  os << '\n';
}

#include <iostream>
#include <iomanip>
#include <ostream>

#include "utils.hpp"
#include "memory.hpp"
#include "video.hpp"

void printMemory(std::ostream& os, const Memory &m) {
  for (size_t i{StartAddress}; i < m.size(); ++i) {
    os << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(m[i]) << ' ';
  }
  os << '\n';
}

void printVideoBuf(std::ostream& os, const VideoBuf &v) {
    for (const auto& p : v) {
        os << std::hex << std::setw(8) << std::setfill('0') << p << ' ';
    }
    os << '\n';
}

void printKeys(std::ostream& os, const Keys &k) {
    for (size_t i{}; i < k.size(); ++i) {
        std::cout << k[i] << ((i + 1) % 4 == 0 ? '\n' : ' ');
    } 
}

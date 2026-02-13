#include <exception>
#include <iostream>
#include <print>

#include "chip8/chip8.hpp"
#include "io/io.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::println("Usage: {} <ROM>", argv[0]);
    return 1;
  }

  try {
    IO io;
    Chip8 chip8;
    chip8.loadROM(argv[1]);

    while (true) {
      io.pollEvents();
      if (io.quitSignal())
        return 0;

      chip8.setKeys(io.getKeys());
      chip8.cycle();

      // if (chip8.sound()) io.playSound();
      // if (chip8.requiresRender())
      io.render(chip8.videoBuf());
    }

  } catch (const std::exception &e) {
    std::println(std::cerr, "Caught exception: {}", e.what());
  } catch (const SDLError &e) {
    std::println(std::cerr, "Caught SDL error: {}", e.msg);
  } catch (...) {
    std::println(std::cerr, "Caught unknown exception...");
  }
}

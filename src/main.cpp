#include <exception>
#include <iostream>
#include <print>

#include "io/io.hpp"

int main(int argc, char **argv) {
  try {
    IO{}.eventLoop();
  } catch (const std::exception &e) {
    std::println(std::cerr, "Caught exception: {}", e.what());
  } catch (const SDLError &e) {
    std::println(std::cerr, "Caught SDL error: {}", e.msg);
  } catch (...) {
    std::println(std::cerr, "Caught unknown exception...");
  }
}

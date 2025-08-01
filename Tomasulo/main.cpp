#include <iostream>
#include "include/CPU.hpp"

int main() {
  CPU cpu;
  cpu.Initialize();
  while (true) {
    cpu.Step();
  }
  return 0;
}
#include <iostream>
#include "include/CPU.hpp"

int main() {
  freopen("info.txt", "w", stderr);
  freopen("../sample/sample.data", "r", stdin);
  CPU cpu;
  cpu.Initialize();
  while (true) {
    cpu.Step();
  }
  return 0;
}
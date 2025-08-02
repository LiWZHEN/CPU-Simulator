#include <iostream>
#include "include/CPU.hpp"

int main() {
  freopen("info.txt", "w", stderr);
  freopen("../testcases/array_test1.data", "r", stdin);
  CPU cpu;
  cpu.Initialize();
  while (true) {
    std::cerr << "-------------------------------[" << cpu.GetClock() << "]-------------------------------------\n";
    cpu.Step();
    cpu.Print();
  }
  return 0;
}
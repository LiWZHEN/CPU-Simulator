#include "../include/ProgramCounter.hpp"

int ProgramCounter::GetPC() const {
  return PC;
}

void ProgramCounter::SetPC(int to) {
  PC = to;
}

void ProgramCounter::NextPC() {
  PC += 4;
}
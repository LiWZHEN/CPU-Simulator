#include "../include/RegFile.hpp"

RegFile::RegFile(const RegFile &other) {
  for (int i = 0; i < 32; ++i) {
    registers[i] = other.registers[i];
  }
}

void RegFile::Modify(const int ind, const int x) {
  registers[ind] = x;
}

int32_t RegFile::GetData(const int ind) {
  return registers[ind];
}
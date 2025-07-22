#include "../include/RegFile.hpp"

RegFile::RegFile(const RegFile &other) {
  for (int i = 0; i < 32; ++i) {
    registers[i] = other.registers[i];
  }
}

void RegFile::Modify(const int32_t ind, const int32_t x) {
  registers[ind] = x;
  if (ind == 0) {
    registers[0] = 0;
  }
}

int32_t RegFile::GetData(const int32_t ind) const {
  return registers[ind];
}
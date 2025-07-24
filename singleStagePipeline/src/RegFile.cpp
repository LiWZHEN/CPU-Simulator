#include "../include/RegFile.hpp"

void RegFile::Modify(const int32_t ind, const int32_t x) {
  registers[ind] = x;
  if (ind == 0) {
    registers[0] = 0;
  }
}

int32_t RegFile::GetData(const int32_t ind) const {
  return registers[ind];
}
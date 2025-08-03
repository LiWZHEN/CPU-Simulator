#include "../include/RegFile.hpp"
#include <iostream>
#include <iomanip>

void RegFile::Modify(const int32_t ind, const int32_t x) {
  registers[ind] = x;
  if (ind == 0) {
    registers[0] = 0;
  }
}

int32_t RegFile::GetData(const int32_t ind) const {
  return registers[ind];
}

void RegFile::Print() {
  std::cerr << "RF:\n|   index    |";
  for (int i = 0; i < 8; ++i) {
    std::cerr << std::dec << std::setw(8) << i << '|';
  }
  std::cerr << "\n|   value    |";
  for (int i = 0; i < 8; ++i) {
    std::cerr << std::hex << std::setw(8) << registers[i] << '|';
  }
  std::cerr << "\n\n";
  std::cerr << "|   index    |";
  for (int i = 8; i < 16; ++i) {
    std::cerr << std::dec << std::setw(8) << i << '|';
  }
  std::cerr << "\n|   value    |";
  for (int i = 8; i < 16; ++i) {
    std::cerr << std::hex << std::setw(8) << registers[i] << '|';
  }
  std::cerr << "\n\n";
  std::cerr << "|   index    |";
  for (int i = 16; i < 24; ++i) {
    std::cerr << std::dec << std::setw(8) << i << '|';
  }
  std::cerr << "\n|   value    |";
  for (int i = 16; i < 24; ++i) {
    std::cerr << std::hex << std::setw(8) << registers[i] << '|';
  }
  std::cerr << "\n\n";
  std::cerr << "|   index    |";
  for (int i = 24; i < 32; ++i) {
    std::cerr << std::dec << std::setw(8) << i << '|';
  }
  std::cerr << "\n|   value    |";
  for (int i = 24; i < 32; ++i) {
    std::cerr << std::hex << std::setw(8) << registers[i] << '|';
  }
  std::cerr << "\n\n";
}
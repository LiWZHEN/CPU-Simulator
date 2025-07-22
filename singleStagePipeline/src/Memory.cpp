#include "../include/Memory.hpp"

void Memory::Add(const int32_t address, const int8_t data) {
  memories[address] = data;
}

int8_t Memory::Get(const int32_t address) const {
  return memories.at(address);
}
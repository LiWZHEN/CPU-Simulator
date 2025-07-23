#include "../include/Memory.hpp"

void Memory::Add(const int32_t address, const uint8_t data) {
  memories[address] = data;
}

uint8_t Memory::Get(const int32_t address) const {
  return memories.at(address);
}

bool Memory::IsEmpty(const int32_t address) const {
  return memories.count(address) == 0;
}
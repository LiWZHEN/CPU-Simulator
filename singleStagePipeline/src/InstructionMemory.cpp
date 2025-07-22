#include "../include/InstructionMemory.hpp"

void InstructionMemory::AddInstruction(const int32_t address, const std::string &instruction) {
  instructions[address] = instruction;
}

std::string InstructionMemory::GetInstruction(const int32_t address) const {
  return instructions.at(address);
}
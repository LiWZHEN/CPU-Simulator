#ifndef INSTRUCTION_MEMORY_HPP
#define INSTRUCTION_MEMORY_HPP

#include <unordered_map>
#include <cstdint>
#include <string>
class InstructionMemory {
  std::unordered_map<int32_t, std::string> instructions;
public:
  InstructionMemory() = default;
  void AddInstruction(const int32_t address, const std::string &instruction);
  std::string GetInstruction(const int32_t address) const;
};
#endif
#include <iostream>
#include <cstdint>
#include "include/Memory.hpp"
#include "include/ProgramCounter.hpp"
#include "include/InstructionDecoder.hpp"


int main() {
  Memory memory;
  ProgramCounter pc;
  InstructionDecoder decoder;
  std::string command_line;
  int32_t address = 0b0;
  while (std::getline(std::cin, command_line)) {
    int ptr = 0;
    bool is_address = false;
    const int len = command_line.length();
    while (ptr < len && command_line[ptr] == ' ') {
      ++ptr;
    }
    if (command_line[ptr] == '@') {
      is_address = true;
      ++ptr;
    }
    if (is_address) {
      address = 0b0;
      for (int i = 7; i >= 0; --i, ++ptr) {
        if (command_line[ptr] >= '0' && command_line[ptr] <= '9') {
          address |= ((command_line[ptr] - '0') << (i << 2));
        } else if (command_line[ptr] >= 'A' && command_line[ptr] <= 'F') {
          address |= ((command_line[ptr] - 'A' + 10) << (i << 2));
        } else {
          throw "Invalid address!";
        }
      }
    } else {
      while (ptr < len) {
        while (ptr < len && command_line[ptr] == ' ') {
          ++ptr;
        }
        if (ptr >= len) {
          break;
        }
        uint8_t single_byte = 0b0;
        if (command_line[ptr] >= '0' && command_line[ptr] <= '9') {
          single_byte |= ((command_line[ptr] - '0') << 4);
        } else if (command_line[ptr] >= 'A' && command_line[ptr] <= 'F') {
          single_byte |= ((command_line[ptr] - 'A' + 10) << 4);
        } else {
          throw "Invalid instruction format!";
        }
        ++ptr;
        if (ptr >= len) {
          throw "Only get half byte!";
        }
        if (command_line[ptr] >= '0' && command_line[ptr] <= '9') {
          single_byte |= (command_line[ptr] - '0');
        } else if (command_line[ptr] >= 'A' && command_line[ptr] <= 'F') {
          single_byte |= (command_line[ptr] - 'A' + 10);
        } else {
          throw "Invalid instruction format!";
        }
        ++ptr;
        memory.Add(address, single_byte);
        address += 1;
      }
    }
  }
  while (pc.GetPC() < 0x2000) {
    int32_t current_pc = pc.GetPC();
    if (memory.IsEmpty(current_pc)) {
      pc.NextPC();
      continue;
    }
    int32_t instruction = (memory.Get(current_pc) | (memory.Get(current_pc + 1) << 8) | (memory.Get(current_pc + 2) << 16) | (memory.Get(current_pc + 3) << 24));
    std::cout << std::hex << current_pc << ": " << decoder.Decode(instruction) << '\n';
    pc.NextPC();
  }
  
  return 0;
}
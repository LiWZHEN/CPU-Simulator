#include "../include/LoadData.hpp"
#include <iostream>

void DataLoader::LoadData(Memory &memory) {
  std::string command;
  int32_t address = 0b0;
  while (std::cin >> command) {
    if (command[0] == '@') {
      address = 0b0;
      for (int i = 7, ptr = 1; i >= 0; --i, ++ptr) {
        if (command[ptr] >= '0' && command[ptr] <= '9') {
          address |= ((command[ptr] - '0') << (i << 2));
        } else if (command[ptr] >= 'A' && command[ptr] <= 'F') {
          address |= ((command[ptr] - 'A' + 10) << (i << 2));
        } else {
          throw InvalidAddress();
        }
      }
    } else {
      uint8_t single_byte = 0b0;
      if (command[0] >= '0' && command[0] <= '9') {
        single_byte |= ((command[0] - '0') << 4);
      } else if (command[0] >= 'A' && command[0] <= 'F') {
        single_byte |= ((command[0] - 'A' + 10) << 4);
      } else {
        throw InvalidInstruction();
      }
      if (command[1] >= '0' && command[1] <= '9') {
        single_byte |= (command[1] - '0');
      } else if (command[1] >= 'A' && command[1] <= 'F') {
        single_byte |= (command[1] - 'A' + 10);
      } else {
        throw InvalidInstruction();
      }
      memory.Add(address, single_byte);
      address += 1;
    }
  }
}
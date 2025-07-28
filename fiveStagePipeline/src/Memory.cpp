#include "../include/Memory.hpp"

void Memory::Connect(RegisterFile *register_file) {
  this->register_file = register_file;
}

void Memory::Add(const int32_t address, const uint8_t data) {
  new_memories[address] = data;
  modify_record.push(ModifyRecordUnit(address, data));
}

int32_t Memory::Get(const int32_t address) const {
  if (old_memories.count(address) == 0) {
    return 0;
  }
  return old_memories.at(address);
}

void Memory::Update() {
  while (!modify_record.empty()) {
    auto top_record = modify_record.front();
    modify_record.pop();
    old_memories[top_record.index] = top_record.value;
  }
}
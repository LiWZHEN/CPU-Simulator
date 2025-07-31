#include "../include/Memory.hpp"
#include <iostream>

void Memory::Connect(LSB *lsb, Decoder *decoder, ROB *rob, RS *rs) {
  this->lsb = lsb;
  this->decoder = decoder;
  this->rob = rob;
  this->rs = rs;
}

void Memory::Add(const int32_t address, const uint8_t data) {
  memories[address] = data;
}

int32_t Memory::Get(const int32_t address) const {
  if (memories.count(address) == 0) {
    return 0;
  }
  return memories.at(address);
}

void Memory::Update() {
  type = task.type;
  address = task.address;
  value = task.value;
}

void Memory::Run() {
  if (type == InstructionType::LB) {
    int32_t data_m = this->Get(address);
    data_m = (data_m << 24) >> 24;
    lsb->GetLoadedData(value, data_m);
    rs->GetLoadedData(value, data_m);
    rob->GetLoadedData(value, data_m);
  } else if (type == InstructionType::LBU) {
    const int32_t data_m = this->Get(address);
    lsb->GetLoadedData(value, data_m);
    rs->GetLoadedData(value, data_m);
    rob->GetLoadedData(value, data_m);
  } else if (type == InstructionType::LH) {
    int32_t data_m = (this->Get(address) | (this->Get(address + 1) << 8));
    data_m = (data_m << 12) >> 12;
    lsb->GetLoadedData(value, data_m);
    rs->GetLoadedData(value, data_m);
    rob->GetLoadedData(value, data_m);
  } else if (type == InstructionType::LHU) {
    const int32_t data_m = (this->Get(address) | (this->Get(address + 1) << 8));
    lsb->GetLoadedData(value, data_m);
    rs->GetLoadedData(value, data_m);
    rob->GetLoadedData(value, data_m);
  } else if (type == InstructionType::LW) {
    const int32_t data_m = (this->Get(address) | (this->Get(address + 1) << 8)
        | (this->Get(address + 2) << 16) | (this->Get(address + 3) << 24));
    lsb->GetLoadedData(value, data_m);
    rs->GetLoadedData(value, data_m);
    rob->GetLoadedData(value, data_m);
  } else if (type == InstructionType::SB) {
    this->Add(address, value & 0xFF);
  } else if (type == InstructionType::SH) {
    this->Add(address, value & 0xFF);
    this->Add(address + 1, (value >> 8) & 0xFF);
  } else if (type == InstructionType::SW) {
    this->Add(address, value & 0xFF);
    this->Add(address + 1, (value >> 8) & 0xFF);
    this->Add(address + 2, (value >> 16) & 0xFF);
    this->Add(address + 3, (value >> 24) & 0xFF);
  }
}

void Memory::SetTask(InstructionType type, int32_t address, int32_t value) {
  task.type = type;
  task.address = address;
  task.value = value;
}
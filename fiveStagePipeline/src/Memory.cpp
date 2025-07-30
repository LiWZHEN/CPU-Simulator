#include "../include/Memory.hpp"

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
  memory_tasks_len = task.memory_tasks_len;
  for (int i = 0; i < memory_tasks_len; ++i) {
    memory_tasks[i] = task.memory_tasks[i];
  }
  task.memory_tasks_len = 0;
}

void Memory::Run() {
  for (int i = 0; i < memory_tasks_len; ++i) {
    MemoryTaskUnit unit_task = memory_tasks[i];
    if (unit_task.type == InstructionType::LB) {
      int32_t data_m = this->Get(unit_task.address);
      data_m = (data_m << 24) >> 24;
      lsb->GetLoadedData(unit_task.value, data_m);
      rs->GetLoadedData(unit_task.value, data_m);
      rob->GetLoadedData(unit_task.value, data_m);
    } else if (unit_task.type == InstructionType::LBU) {
      const int32_t data_m = this->Get(unit_task.address);
      lsb->GetLoadedData(unit_task.value, data_m);
      rs->GetLoadedData(unit_task.value, data_m);
      rob->GetLoadedData(unit_task.value, data_m);
    } else if (unit_task.type == InstructionType::LH) {
      int32_t data_m = (this->Get(unit_task.address) | (this->Get(unit_task.address + 1) << 8));
      data_m = (data_m << 12) >> 12;
      lsb->GetLoadedData(unit_task.value, data_m);
      rs->GetLoadedData(unit_task.value, data_m);
      rob->GetLoadedData(unit_task.value, data_m);
    } else if (unit_task.type == InstructionType::LHU) {
      const int32_t data_m = (this->Get(unit_task.address) | (this->Get(unit_task.address + 1) << 8));
      lsb->GetLoadedData(unit_task.value, data_m);
      rs->GetLoadedData(unit_task.value, data_m);
      rob->GetLoadedData(unit_task.value, data_m);
    } else if (unit_task.type == InstructionType::LW) {
      const int32_t data_m = (this->Get(unit_task.address) | (this->Get(unit_task.address + 1) << 8)
          | (this->Get(unit_task.address + 2) << 16) | (this->Get(unit_task.address + 3) << 24));
      lsb->GetLoadedData(unit_task.value, data_m);
      rs->GetLoadedData(unit_task.value, data_m);
      rob->GetLoadedData(unit_task.value, data_m);
    } else if (unit_task.type == InstructionType::SB) {
      this->Add(unit_task.address, unit_task.value & 0xFF);
    } else if (unit_task.type == InstructionType::SH) {
      this->Add(unit_task.address, unit_task.value & 0xFF);
      this->Add(unit_task.address + 1, (unit_task.value >> 8) & 0xFF);
    } else if (unit_task.type == InstructionType::SW) {
      this->Add(unit_task.address, unit_task.value & 0xFF);
      this->Add(unit_task.address + 1, (unit_task.value >> 8) & 0xFF);
      this->Add(unit_task.address + 2, (unit_task.value >> 16) & 0xFF);
      this->Add(unit_task.address + 3, (unit_task.value >> 24) & 0xFF);
    }
  }
}

void Memory::SetTask(InstructionType type, int32_t address, int32_t value) {
  task.memory_tasks[task.memory_tasks_len++] = {type, address, value};
}
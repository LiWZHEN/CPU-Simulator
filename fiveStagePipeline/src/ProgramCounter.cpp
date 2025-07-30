#include "../include/ProgramCounter.hpp"

void ProgramCounter::Connect(Memory *memory, Decoder *decoder) {
  this->memory = memory;
  this->decoder = decoder;
}

void ProgramCounter::SetPCTask(int32_t to) {
  if (!task.predict_failed) {
    task.force_jump = true;
    task.jump_to = to;
    task.wait_for_next = false;
  }
}

void ProgramCounter::SetPCWait() {
  task.wait_for_next = true;
}

int32_t ProgramCounter::GetMachineCode() {
  return (memory->Get(pc) | (memory->Get(pc + 1) << 8) | (memory->Get(pc + 2) << 16) | (memory->Get(pc + 3) << 24));
}

void ProgramCounter::PredictFailed(int32_t another_branch_pos) {
  task.predict_failed = true;
  task.jump_to = another_branch_pos;
  task.force_jump = false;
  task.rob_is_full = false;
  task.wait_for_next = false;
}

void ProgramCounter::Update() {
  wait_for_next = task.wait_for_next;
  if (wait_for_next) {
    return;
  }
  predict_failed = task.predict_failed;
  rob_is_full = task.rob_is_full;
  force_jump = task.force_jump;
  jump_to = task.jump_to;

  task.predict_failed = false;
  task.rob_is_full = false;
  task.force_jump = false;
  task.jump_to = -1;
}

void ProgramCounter::Run() {
  if (predict_failed || force_jump) {
    pc = jump_to;
  }
  if (wait_for_next || rob_is_full) {
    return;
  }
  int32_t machine_code = GetMachineCode();
  decoder->SetFromPC(machine_code);
  decoder->SetCurrentPC(pc);
  pc += 4;
}
#include "../include/ArithmeticLogicUnit.hpp"

void ALU::SetTask(bool destination_is_rs, bool destination_is_lsb,
    int32_t index, op operation, int32_t v1, int32_t v2) {
  new_task.destination_is_rs = destination_is_rs;
  new_task.destination_is_lsb = destination_is_lsb;
  new_task.index = index;
  new_task.operation = operation;
  new_task.v1 = v1;
  new_task.v2 = v2;
}

void ALU::Update() {
  old_task = new_task;
}

void ALU::Run() {
  if (old_task.operation == ADD) {
    old_task.ans = old_task.v1 + old_task.v2;
  } else if (old_task.operation == SUB) {
    old_task.ans = old_task.v1 - old_task.v2;
  } else if (old_task.operation == AND) {
    old_task.ans = old_task.v1 & old_task.v2;
  } else if (old_task.operation == OR) {
    old_task.ans = old_task.v1 | old_task.v2;
  } else if (old_task.operation == XOR) {
    old_task.ans = old_task.v1 ^ old_task.v2;
  } else if (old_task.operation == SLL) {
    old_task.ans = old_task.v1 << old_task.v2;
  } else if (old_task.operation == SRL) {
    const uint32_t uv1 = old_task.v1;
    old_task.ans = uv1 >> old_task.v2;
  } else if (old_task.operation == SRA) {
    old_task.ans = old_task.v1 >> old_task.v2;
  } else if (old_task.operation == SLT) {
    old_task.ans = (old_task.v1 < old_task.v2) ? 1 : 0;
  } else if (old_task.operation == SLTU) {
    const uint32_t uv1 = old_task.v1, uv2 = old_task.v2;
    old_task.ans = (uv1 < uv2) ? 1 : 0;
  }
}

const ALUTask ALU::GetOld() const{
  return old_task;
}
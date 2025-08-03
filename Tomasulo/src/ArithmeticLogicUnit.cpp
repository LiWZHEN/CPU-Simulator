#include "../include/ArithmeticLogicUnit.hpp"
#include <iostream>

void ALU::Connect(ROB *rob, RS *rs, LSB *lsb, ProgramCounter *pc) {
  this->rob = rob;
  this->rs = rs;
  this->lsb = lsb;
  this->pc = pc;
}

void ALU::SetTask(int32_t destination, InstructionType type, int32_t v1, int32_t v2) {
  task.destination = destination;
  task.type = type;
  task.v1 = v1;
  task.v2 = v2;
}

void ALU::Update() {
  predict_failed = task.predict_failed;
  if (predict_failed) {
    type = InstructionType::NONE;
    task.predict_failed = false;
    task.type = InstructionType::NONE;
    return;
  }
  type = task.type;
  destination = task.destination;
  v1 = task.v1;
  v2 = task.v2;
}

void ALU::SetPredictFault() {
  task.predict_failed = true;
}

void ALU::Run() {
  if (type == InstructionType::NONE) {
    rob->SetFromALU(InstructionType::NONE, 0, 0);
    rs->SetFromALU(InstructionType::NONE, 0, 0);
    lsb->SetFromALU(InstructionType::NONE, 0, 0);
    return;
  }
  bool is_arithmetic = false;
  int32_t ans;
  if (type == InstructionType::ADD) {
    ans = v1 + v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SUB) {
    ans = v1 - v2;
    is_arithmetic = true;
  } else if (type == InstructionType::AND) {
    ans = v1 & v2;
    is_arithmetic = true;
  } else if (type == InstructionType::OR) {
    ans = v1 | v2;
    is_arithmetic = true;
  } else if (type == InstructionType::XOR) {
    ans = v1 ^ v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SLL) {
    ans = v1 << v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SRL) {
    const uint32_t uv1 = v1;
    ans = uv1 >> v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SRA) {
    ans = v1 >> v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SLT) {
    ans = (v1 < v2) ? 1 : 0;
    is_arithmetic = true;
  } else if (type == InstructionType::SLTU) {
    const uint32_t uv1 = v1, uv2 = v2;
    ans = (uv1 < uv2) ? 1 : 0;
    is_arithmetic = true;
  } else if (type == InstructionType::ADDI) {
    ans = v1 + v2;
    is_arithmetic = true;
  } else if (type == InstructionType::ANDI) {
    ans = v1 & v2;
    is_arithmetic = true;
  } else if (type == InstructionType::ORI) {
    ans = v1 | v2;
    is_arithmetic = true;
  } else if (type == InstructionType::XORI) {
    ans = v1 ^ v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SLLI) {
    ans = v1 << v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SRLI) {
    const uint32_t uv1 = v1;
    ans = uv1 >> v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SRAI) {
    ans = v1 >> v2;
    is_arithmetic = true;
  } else if (type == InstructionType::SLTI) {
    ans = (v1 < v2) ? 1 : 0;
    is_arithmetic = true;
  } else if (type == InstructionType::SLTIU) {
    const uint32_t uv1 = v1, uv2 = v2;
    ans = (uv1 < uv2) ? 1 : 0;
    is_arithmetic = true;
  }
  if (is_arithmetic) {
    rob->SetFromALU(type, ans, destination);
    rs->SetFromALU(type, ans, destination);
    lsb->SetFromALU(type, ans, destination);
    return;
  }

  if (type == InstructionType::LB || type == InstructionType::LBU
      || type == InstructionType::LH || type == InstructionType::LHU
      || type == InstructionType::LW) {
    ans = v1 + v2;
    lsb->SetFromALU(type, ans, destination);
    rs->SetFromALU(InstructionType::NONE, 0, 0);
    rob->SetFromALU(InstructionType::NONE, 0, 0);
    return;
  }

  if (type == InstructionType::SB || type == InstructionType::SH
      || type == InstructionType::SW) {
    ans = v1 + v2;
    lsb->SetFromALU(InstructionType::NONE, 0, 0);
    rs->SetFromALU(InstructionType::NONE, 0, 0);
    rob->SetFromALU(type, ans, destination);
    return;
  }

  bool is_branch = false;
  if (type == InstructionType::BEQ) {
    ans = (v1 == v2) ? 1 : 0;
    is_branch = true;
  } else if (type == InstructionType::BGE) {
    ans = (v1 >= v2) ? 1 : 0;
    is_branch = true;
  } else if (type == InstructionType::BGEU) {
    uint32_t uv1 = v1, uv2 = v2;
    ans = (uv1 >= uv2) ? 1 : 0;
    is_branch = true;
  } else if (type == InstructionType::BLT) {
    ans = (v1 < v2) ? 1 : 0;
    is_branch = true;
  } else if (type == InstructionType::BLTU) {
    uint32_t uv1 = v1, uv2 = v2;
    ans = (uv1 < uv2) ? 1 : 0;
    is_branch = true;
  } else if (type == InstructionType::BNE) {
    ans = (v1 != v2) ? 1 : 0;
    is_branch = true;
  }
  if (is_branch) {
    rob->SetFromALU(type, ans, destination);
    rs->SetFromALU(InstructionType::NONE, 0, 0);
    lsb->SetFromALU(InstructionType::NONE, 0, 0);
    return;
  }

  if (type == InstructionType::JALR) {
    ans = v1 + v2;
    rob->SetFromALU(InstructionType::NONE, 0, 0);
    rs->SetFromALU(InstructionType::NONE, 0, 0);
    lsb->SetFromALU(InstructionType::NONE, 0, 0);
    pc->SetPCTask(ans);
  } else if (type == InstructionType::LUI) {
    ans = v1 + v2;
    rob->SetFromALU(InstructionType::LUI, ans, destination);
    rs->SetFromALU(InstructionType::LUI, ans, destination);
    lsb->SetFromALU(InstructionType::LUI, ans, destination);
  } else if (type == InstructionType::AUIPC) {
    ans = v1 + v2;
    rob->SetFromALU(InstructionType::AUIPC, ans, destination);
    rs->SetFromALU(InstructionType::AUIPC, ans, destination);
    lsb->SetFromALU(InstructionType::AUIPC, ans, destination);
  } else if (type == InstructionType::JAL) {
    ans = v1 + v2;
    rob->SetFromALU(InstructionType::JAL, ans, destination);
    rs->SetFromALU(InstructionType::JAL, ans, destination);
    lsb->SetFromALU(InstructionType::JAL, ans, destination);
  } else {
    throw "Unexpected operation!";
  }
}
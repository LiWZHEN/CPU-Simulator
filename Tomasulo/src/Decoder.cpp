#include "../include/Decoder.hpp"
#include "../include/Exception.hpp"
#include <iostream>


void Decoder::Connect(ROB *rob, RS *rs, LSB *lsb, RegisterFile* rf,
    ProgramCounter *pc, Predictor *predictor) {
  this->rob = rob;
  this->rs = rs;
  this->lsb = lsb;
  this->rf = rf;
  this->pc = pc;
  this->predictor = predictor;
}

void Decoder::SetFromPC(int32_t machine_code) {
  task.machine_code = machine_code;
}

void Decoder::SetCurrentPC(int32_t current_pc) {
  task.current_pc = current_pc;
}

void Decoder::PredictFailed() {
  task.predict_failed = true;
  task.rob_is_full = false;
}

void Decoder::CommitMessageFromROB(int32_t rob_ind, int32_t value) {
  task.commit_message[task.commit_message_len++] = {rob_ind, value};
}

void Decoder::PassRF(int32_t rf_data[], int32_t rf_dependence[]) {
  for (int i = 0; i < 32; ++i) {
    task.rf_data[i] = rf_data[i];
    task.rf_dependence[i] = rf_dependence[i];
  }
}

void Decoder::ROBFull() {
  task.rob_is_full = true;
}

void Decoder::Update() {
  if (task.predict_failed) {
    predict_falied = true;
    task.predict_failed = false;
    task.commit_message_len = 0;
    task.discard_this = false;
    return;
  }
  if (task.discard_this) {
    task.machine_code = 0;
    task.discard_this = false;
  }
  rob_is_full = task.rob_is_full;
  task.rob_is_full = false;
  predict_falied = false;
  if (rob_is_full) {
    task.commit_message_len = 0; 
    return;
  }
  new_dependence_rd = task.new_dependence_rd;
  new_dependence_rob_ind = task.new_dependence_rob_ind;
  task.new_dependence_rd = -1;
  task.new_dependence_rob_ind = -1;
  machine_code = task.machine_code;
  current_pc = task.current_pc;
  for (int i = 0; i < 32; ++i) {
    rf_data[i] = task.rf_data[i];
    rf_dependence[i] = task.rf_dependence[i];
  }
  commit_message_len = task.commit_message_len;
  for (int i = 0; i < commit_message_len; ++i) {
    commit_message[i] = task.commit_message[i];
  }
  task.commit_message_len = 0;
}

std::string IntToString(uint32_t x) {
  std::string ans;
  for (int i = 28; i >= 0; i -= 4) {
    uint32_t mask = 0b1111 << i;
    uint8_t byte = (x & mask) >> i;
    if (byte < 10) {
      ans.append(1, (byte + '0'));
    } else {
      ans.append(1, (byte - 10 + 'A'));
    }
  }
  return ans;
}

uint32_t StringToInt(const std::string &str) {
  uint32_t ans = 0;
  for (int i = 0; i < str.length(); ++i) {
    uint32_t half_byte;
    if (str[i] >= '0' && str[i] <= '9') {
      half_byte = str[i] - '0';
    } else if (str[i] >= 'A' && str[i] <= 'F') {
      half_byte = str[i] - 'A' + 10;
    } else {
      throw InvalidHexString();
    }
    ans = (ans << 4) | half_byte;
  }
  return ans;
}

void Decoder::Decode_R() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      funct7 = (code >> 25) & 0b1111111;
  if (rd == 0) {
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    rs->PassRS(-1, -1);
    return;
  }
  InstructionType type;
  int32_t Q1, Q2, V1, V2;
  Q1 = rf_dependence[rs1];
  if (Q1 == -1) {
    V1 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q1) {
        Q1 = -1;
        V1 = commit_message[i].value;
        break;
      }
    }
  }
  Q2 = rf_dependence[rs2];
  if (Q2 == -1) {
    V2 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q2) {
        Q2 = -1;
        V2 = commit_message[i].value;
        break;
      }
    }
  }
  if (new_dependence_rd != -1) {
    if (rs1 == new_dependence_rd) {
      Q1 = new_dependence_rob_ind;
    }
    if (rs2 == new_dependence_rd) {
      Q2 = new_dependence_rob_ind;
    }
  }
  switch (funct3) {
  case 0b000:
    if (funct7 == 0b0000000) {
      type = ADD;
    } else if (funct7 == 0b0100000) {
      type = SUB;
    } else {
      throw InvalidFunction();
    }
    break;
  case 0b111:
    type = AND;
    break;
  case 0b110:
    type = OR;
    break;
  case 0b100:
    type = XOR;
    break;
  case 0b001:
    type = SLL;
    break;
  case 0b101:
    if (funct7 == 0b0000000) {
      type = SRL;
    } else if (funct7 == 0b0100000) {
      type = SRA;
    } else {
      throw InvalidFunction();
    }
    break;
  case 0b010:
    type = SLT;
    break;
  case 0b011:
    type = SLTU;
    break;
  default:
    throw InvalidFunction();
  }
  rs->SetFromDecoder(type, V1, V2, Q1, Q2, -1);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
  rob->SetFromDecoder(type, rd, 0, false);
  rs->PassRS(rs1, rs2);
}

void Decoder::Decode_IA() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111;
  if (rd == 0) {
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    rs->PassRS(-1, -1);
    return;
  }
  InstructionType type;
  int32_t Q1, V1, imm;
  Q1 = rf_dependence[rs1];
  if (Q1 == -1) {
    V1 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q1) {
        Q1 = -1;
        V1 = commit_message[i].value;
        break;
      }
    }
  }
  if (new_dependence_rd != -1) {
    if (rs1 == new_dependence_rd) {
      Q1 = new_dependence_rob_ind;
    }
  }
  if (funct3 == 0b001) {
    const int32_t funct7 = (code >> 25) & 0b1111111;
    imm = (code >> 20) & 0b11111;
    type = SLLI;
  } else if (funct3 == 0b101) {
    const int32_t funct7 = (code >> 25) & 0b1111111;
    imm = (code >> 20) & 0b11111;
    if (funct7 == 0b0000000) {
      type = SRLI;
    } else if (funct7 == 0b0100000) {
      type = SRAI;
    } else {
      throw InvalidFunction();
    }
  } else {
    imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
    switch (funct3) {
    case 0b000:
      type = ADDI;
      break;
    case 0b111:
      type = ANDI;
      break;
    case 0b110:
      type = ORI;
      break;
    case 0b100:
      type = XORI;
      break;
    case 0b010:
      type = SLTI;
      break;
    case 0b011:
      type = SLTIU;
      break;
    default:
      throw InvalidFunction();
    }
  }
  rs->SetFromDecoder(type, V1, imm, Q1, -1, -1);
  rs->PassRS(rs1, -1);
  rob->SetFromDecoder(type, rd, 0, false);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
}

void Decoder::Decode_IM() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  if (rd == 0) {
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    rs->PassRS(-1, -1);
    return;
  }
  InstructionType type;
  int32_t V1, Q1;
  Q1 = rf_dependence[rs1];
  if (Q1 == -1) {
    V1 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q1) {
        Q1 = -1;
        V1 = commit_message[i].value;
        break;
      }
    }
  }
  if (new_dependence_rd != -1) {
    if (rs1 == new_dependence_rd) {
      Q1 = new_dependence_rob_ind;
    }
  }
  switch (funct3) {
  case 0b000:
    type = LB;
    break;
  case 0b100:
    type = LBU;
    break;
  case 0b001:
    type = LH;
    break;
  case 0b101:
    type = LHU;
    break;
  case 0b010:
    type = LW;
    break;
  default:
    throw InvalidFunction();
  }
  rs->SetFromDecoder(type, V1, imm, Q1, -1, -1);
  rs->PassRS(rs1, -1);
  rob->SetFromDecoder(type, rd, 0, false);
  lsb->SetFromDecoder(type, -1, -1, -1, false);
}

void Decoder::Decode_IC() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  if (funct3 != 0b000) {
    throw InvalidFunction();
  }
  int32_t V1, Q1;
  Q1 = rf_dependence[rs1];
  if (Q1 == -1) {
    V1 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q1) {
        Q1 = -1;
        V1 = commit_message[i].value;
        break;
      }
    }
  }
  if (new_dependence_rd != -1) {
    if (rs1 == new_dependence_rd) {
      Q1 = new_dependence_rob_ind;
    }
  }
  if (rd == 0) {
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);    
  } else {
    rob->SetFromDecoder(InstructionType::JALR, rd, current_pc + 4, true);
  }
  rs->PassRS(rs1, -1);
  rs->SetFromDecoder(InstructionType::JALR, V1, imm, Q1, -1, -1);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
  pc->SetPCWait();
  task.discard_this = true;
}

void Decoder::Decode_S() {
  const int32_t code = machine_code;
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 25) & 0b1111111) << 5) | ((code >> 7) & 0b11111)) << 20) >> 20;
  InstructionType type;
  int32_t V1, V2, Q1, Q2;
  Q1 = rf_dependence[rs1];
  if (Q1 == -1) {
    V1 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q1) {
        Q1 = -1;
        V1 = commit_message[i].value;
        break;
      }
    }
  }
  if (new_dependence_rd != -1) {
    if (rs1 == new_dependence_rd) {
      Q1 = new_dependence_rob_ind;
    }
  }
  switch (funct3) {
  case 0b000:
    type = SB;
    break;
  case 0b001:
    type = SH;
    break;
  case 0b010:
    type = SW;
    break;
  default:
    throw InvalidFunction();
  }
  rs->SetFromDecoder(type, V1, imm, Q1, -1, -1);
  rs->PassRS(rs1, -1);
  rob->SetFromDecoder(type, rs2, -1, false);
  lsb->SetFromDecoder(type, -1, -1, -1, false);
}

void Decoder::Decode_B() {
  const int32_t code = machine_code;
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 12) | (((code >> 7) & 0b1) << 11) | (((code >> 25) & 0b111111) << 5) | (((code >> 8) & 0b1111) << 1)) << 19) >> 19;
  InstructionType type;
  int32_t V1, V2, Q1, Q2;
  Q1 = rf_dependence[rs1];
  if (Q1 == -1) {
    V1 = rf_data[rs1];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q1) {
        Q1 = -1;
        V1 = commit_message[i].value;
        break;
      }
    }
  }
  Q2 = rf_dependence[rs2];
  if (Q2 == -1) {
    V2 = rf_data[rs2];
  } else {
    for (int i = 0; i < commit_message_len; ++i) {
      if (commit_message[i].rob_ind == Q2) {
        Q2 = -1;
        V2 = commit_message[i].value;
        break;
      }
    }
  }
  if (new_dependence_rd != -1) {
    if (rs1 == new_dependence_rd) {
      Q1 = new_dependence_rob_ind;
    }
    if (rs2 == new_dependence_rd) {
      Q2 = new_dependence_rob_ind;
    }
  }
  switch (funct3) {
  case 0b000:
    type = BEQ;
    break;
  case 0b101:
    type = BGE;
    break;
  case 0b111:
    type = BGEU;
    break;
  case 0b100:
    type = BLT;
    break;
  case 0b110:
    type = BLTU;
    break;
  case 0b001:
    type = BNE;
    break;
  default:
    throw InvalidFunction();
  }
  bool predict_jump = predictor->QueryJump(current_pc);
  if (predict_jump) {
    pc->SetPCTask(current_pc + imm);
    task.discard_this = true;
    rob->SetFromDecoder(type, current_pc + 4, current_pc << 1 | 1, false);
  } else {
    rob->SetFromDecoder(type, current_pc + imm, current_pc << 1, false);
  }
  rs->SetFromDecoder(type, V1, V2, Q1, Q2, -1);
  rs->PassRS(rs1, rs2);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
}

void Decoder::Decode_J() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 20) | (((code >> 12) & 0b11111111) << 12)
      | (((code >> 20) & 0b1) << 11) | (((code >> 21) & 0b1111111111) << 1)) << 11) >> 11;
  pc->SetPCTask(current_pc + imm);
  task.discard_this = true;
  if (rd == 0) {
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    rs->PassRS(-1, -1);
    return;
  }
  rob->SetFromDecoder(InstructionType::JAL, rd, current_pc + 4, true);
  rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
  rs->PassRS(-1, -1);
}

void Decoder::Decode_AUIPC() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  const int32_t new_pc = current_pc + imm;
  if (rd == 0) {
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    rs->SetFromDecoder(InstructionType::NONE, new_pc, 0, -1, -1, -1);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    rs->PassRS(-1, -1);
    return;
  }
  rob->SetFromDecoder(InstructionType::AUIPC, rd, new_pc, true);
  rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
  rs->PassRS(-1, -1);
  return;
}

void Decoder::Decode_LUI() {
  const int32_t code = machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  if (rd == 0) {
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    rs->PassRS(-1, -1);
    return;
  }
  rob->SetFromDecoder(InstructionType::LUI, rd, imm, false);
  rs->SetFromDecoder(InstructionType::LUI, 0, imm, -1, -1, -1);
  lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
  rs->PassRS(-1, -1);
  return;
}

void Decoder::Run() {
  if (machine_code == 0 || predict_falied) {
    rob->SetFromDecoder(InstructionType::NONE, 0, 0, false);
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    return;
  }
  if (machine_code == 0x0ff00513) {
    rob->SetFromDecoder(InstructionType::EXIT, 0, 0, true);
    rs->SetFromDecoder(InstructionType::NONE, 0, 0, 0, 0, 0);
    lsb->SetFromDecoder(InstructionType::NONE, 0, 0, 0, false);
    pc->SetPCWait();
    task.discard_this = true;
    return;
  }
  const int32_t opcode = machine_code & 0b1111111;
  switch (opcode) {
  case 0b0110011:
    Decode_R();
    return;
  case 0b0100011:
    Decode_S();
    return;
  case 0b1100011:
    Decode_B();
    return;
  case 0b1101111:
    Decode_J();
    return;
  case 0b0010111:
    Decode_AUIPC();
    return;
  case 0b0110111:
    Decode_LUI();
    return;
  case 0b0010011:
    Decode_IA();
    return;
  case 0b0000011:
    Decode_IM();
    return;
  case 0b1100111:
    Decode_IC();
    return;
  case 0b1110011:
    throw "Unexpected I-type instruction!";
  default:
    throw NoMatchedType();
  }
}

void Decoder::SetNewDependence(int32_t rd, int32_t dependence) {
  task.new_dependence_rd = rd;
  task.new_dependence_rob_ind = dependence;
}
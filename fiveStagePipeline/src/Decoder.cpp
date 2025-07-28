#include "../include/Decoder.hpp"
#include "../include/Exception.hpp"
#include <iostream>


void Decoder::Connect(ROB *rob, RS *rs, LSB *lsb, RegisterFile* register_file) {
  this->rob = rob;
  this->rs = rs;
  this->lsb = lsb;
  this->register_file = register_file;
}

void Decoder::SetTask(int32_t machine_code) {
  new_task.machine_code = machine_code;
}

void Decoder::Update() {
  old_task = new_task;
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
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      funct7 = (code >> 25) & 0b1111111;
  switch (funct3) {
  case 0b000:
    if (funct7 == 0b0000000) {
      new_task.type = ADD;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.rs2 = rs2;
      new_task.Q1 = register_file->GetDependence(rs1);
      new_task.Q2 = register_file->GetDependence(rs2);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      if (new_task.Q2 == -1) {
        new_task.V2 = register_file->GetData(rs2);
      }
      return;
    } else if (funct7 == 0b0100000) {
      new_task.type = SUB;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.rs2 = rs2;
      new_task.Q1 = register_file->GetDependence(rs1);
      new_task.Q2 = register_file->GetDependence(rs2);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      if (new_task.Q2 == -1) {
        new_task.V2 = register_file->GetData(rs2);
      }
      return;
    } else {
      throw InvalidFunction();
    }
  case 0b111:
    new_task.type = AND;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    return;
  case 0b110:
    new_task.type = OR;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    return;
  case 0b100:
    new_task.type = XOR;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    return;
  case 0b001:
    new_task.type = SLL;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    return;
  case 0b101:
    if (funct7 == 0b0000000) {
      new_task.type = SRL;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.rs2 = rs2;
      new_task.Q1 = register_file->GetDependence(rs1);
      new_task.Q2 = register_file->GetDependence(rs2);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      if (new_task.Q2 == -1) {
        new_task.V2 = register_file->GetData(rs2);
      }
      return;
    } else if (funct7 == 0b0100000) {
      new_task.type = SRA;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.rs2 = rs2;
      new_task.Q1 = register_file->GetDependence(rs1);
      new_task.Q2 = register_file->GetDependence(rs2);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      if (new_task.Q2 == -1) {
        new_task.V2 = register_file->GetData(rs2);
      }
      return;
    } else {
      throw InvalidFunction();
    }
  case 0b010:
    new_task.type = SLT;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    return;
  case 0b011:
    new_task.type = SLTU;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    return;
  default:
    throw InvalidFunction();
  }
}

void Decoder::Decode_IA() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111;
  if (funct3 == 0b001) {
    const int32_t imm = (code >> 20) & 0b11111,
        funct7 = (code >> 25) & 0b1111111;
    new_task.type = SLLI;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.Q1 = register_file->GetDependence(rs1);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    new_task.imm = imm;
    return;
  } else if (funct3 == 0b101) {
    const int32_t imm = (code >> 20) & 0b11111,
        funct7 = (code >> 25) & 0b1111111;
    if (funct7 == 0b0000000) {
      new_task.type = SRLI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    } else if (funct7 == 0b0100000) {
      new_task.type = SRAI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    } else {
      throw InvalidFunction();
    }
  } else {
    const int32_t imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
    switch (funct3) {
    case 0b000:
      new_task.type = ADDI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    case 0b111:
      new_task.type = ANDI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    case 0b110:
      new_task.type = ORI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    case 0b100:
      new_task.type = XORI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    case 0b010:
      new_task.type = SLTI;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    case 0b011:
      new_task.type = SLTIU;
      new_task.destination = rd;
      new_task.rs1 = rs1;
      new_task.Q1 = register_file->GetDependence(rs1);
      if (new_task.Q1 == -1) {
        new_task.V1 = register_file->GetData(rs1);
      }
      new_task.imm = imm;
      return;
    default:
      throw InvalidFunction();
    }
  }
}

void Decoder::Decode_IM() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  switch (funct3) {
  case 0b000:
    new_task.type = LB;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.Q1 = register_file->GetDependence(rs1);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    new_task.imm = imm;
    return;
  case 0b100:
    new_task.type = LBU;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.Q1 = register_file->GetDependence(rs1);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    new_task.imm = imm;
    return;
  case 0b001:
    new_task.type = LH;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.Q1 = register_file->GetDependence(rs1);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    new_task.imm = imm;
    return;
  case 0b101:
    new_task.type = LHU;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.Q1 = register_file->GetDependence(rs1);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    new_task.imm = imm;
    return;
  case 0b010:
    new_task.type = LW;
    new_task.destination = rd;
    new_task.rs1 = rs1;
    new_task.Q1 = register_file->GetDependence(rs1);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    new_task.imm = imm;
    return;
  default:
    throw InvalidFunction();
  }
}

void Decoder::Decode_IC() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  if (funct3 != 0b000) {
    throw InvalidFunction();
  }
  new_task.type = JALR;
  new_task.destination = rd;
  new_task.rs1 = rs1;
  new_task.Q1 = register_file->GetDependence(rs1);
  if (new_task.Q1 == -1) {
    new_task.V1 = register_file->GetData(rs1);
  }
  new_task.imm = imm;
  return;
}

void Decoder::Decode_IO() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20; 
  if (funct3 != 0b000) {
    throw InvalidFunction();
  }
  if (imm == 0) {
    throw "ebreak";
  }
  if (imm == 1) {
    throw "ecall";
  }
  throw InvalidFunction();
}

void Decoder::Decode_S() {
  const int32_t code = new_task.machine_code;
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 25) & 0b1111111) << 5) | ((code >> 7) & 0b11111)) << 20) >> 20;
  switch (funct3) {
  case 0b000:
    new_task.type = SB;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b001:
    new_task.type = SH;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b010:
    new_task.type = SW;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  default:
    throw InvalidFunction();
  }
}

void Decoder::Decode_B() {
  const int32_t code = new_task.machine_code;
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 12) | (((code >> 7) & 0b1) << 11) | (((code >> 25) & 0b111111) << 5) | (((code >> 8) & 0b1111) << 1)) << 19) >> 19;
  switch (funct3) {
  case 0b000:
    new_task.type = BEQ;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b101:
    new_task.type = BGE;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b111:
    new_task.type = BGEU;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b100:
    new_task.type = BLT;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b110:
    new_task.type = BLTU;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  case 0b001:
    new_task.type = BNE;
    new_task.rs1 = rs1;
    new_task.rs2 = rs2;
    new_task.Q1 = register_file->GetDependence(rs1);
    new_task.Q2 = register_file->GetDependence(rs2);
    if (new_task.Q1 == -1) {
      new_task.V1 = register_file->GetData(rs1);
    }
    if (new_task.Q2 == -1) {
      new_task.V2 = register_file->GetData(rs2);
    }
    new_task.imm = imm;
    return;
  default:
    throw InvalidFunction();
  }
}

void Decoder::Decode_J() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 20) | (((code >> 12) & 0b11111111) << 12)
      | (((code >> 20) & 0b1) << 11) | (((code >> 21) & 0b1111111111) << 1)) << 11) >> 11;
  new_task.type = JAL;
  new_task.destination = rd;
  new_task.imm = imm;
  return;
}

void Decoder::Decode_AUIPC() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  new_task.type = AUIPC;
  new_task.destination = rd;
  new_task.imm = imm;
  return;
}

void Decoder::Decode_LUI() {
  const int32_t code = new_task.machine_code;
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  new_task.type = LUI;
  new_task.destination = rd;
  new_task.imm = imm;
  return;
}

void Decoder::Run() {
  const int32_t code = new_task.machine_code;
  const int32_t opcode = code & 0b1111111;
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
    Decode_IO();
    return;
  default:
    throw NoMatchedType();
  }
}
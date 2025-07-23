#include "../include/InstructionDecoder.hpp"

std::string IntToString(int32_t x, int8_t len) {
  std::string ans;
  for (int l = len - 1; l >= 0; --l) {
    if (x & (1 << l)) {
      ans += "1";
    } else {
      ans += "0";
    }
  }
  return ans;
}

int32_t StringToInt(const std::string &str) {
  int32_t ans = 0;
  for (int i = 0; i < str.length(); ++i) {
    ans = str[i] == '1' ? (ans << 1 | 1) : (ans << 1);
  }
  return ans;
}

std::string InstructionDecoder::Decode_R(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      funct7 = (code >> 25) & 0b1111111;
  switch (funct3) {
  case 0b000:
    if (funct7 == 0b0000000) {
      return "add " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
    } else if (funct7 == 0b0100000) {
      return "sub " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
    } else {
      throw "Invalid function!";
    }
  case 0b111:
    return "and " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
  case 0b110:
    return "or " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
  case 0b100:
    return "xor " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
  case 0b001:
    return "sll " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
  case 0b101:
    if (funct7 == 0b0000000) {
      return "srl " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
    } else if (funct7 == 0b0100000) {
      return "sra " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
    } else {
      throw "Invalid function!";
    }
  case 0b010:
    return "slt " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
  case 0b011:
    return "sltu " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(rs2, 5);
  default:
    throw "Invalid function!";
  }
}

std::string InstructionDecoder::Decode_IA(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111;
  if (funct3 == 0b001) {
    const int32_t imm = (code >> 20) & 0b11111,
        funct7 = (code >> 25) & 0b1111111;
    return "slli " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 5);
  } else if (funct3 == 0b101) {
    const int32_t imm = (code >> 20) & 0b11111,
        funct7 = (code >> 25) & 0b1111111;
    if (funct7 == 0b0000000) {
      return "srli " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 5);
    } else if (funct7 == 0b0100000) {
      return "srai " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 5);
    } else {
      throw "Invalid function!";
    }
  } else {
    const int32_t imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
    switch (funct3) {
    case 0b000:
      return "addi " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
    case 0b111:
      return "andi " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
    case 0b110:
      return "ori " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
    case 0b100:
      return "xori " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
    case 0b010:
      return "slti " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
    case 0b011:
      return "sltiu " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
    default:
      throw "Invalid Function!";
    }
  }
}

std::string InstructionDecoder::Decode_IM(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  switch (funct3) {
  case 0b000:
    return "lb " + IntToString(rd, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  case 0b100:
    return "lbu " + IntToString(rd, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  case 0b001:
    return "lh " + IntToString(rd, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  case 0b101:
    return "lhu " + IntToString(rd, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  case 0b010:
    return "lw " + IntToString(rd, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  default:
    throw "Invalid function!";
  }
}

std::string InstructionDecoder::Decode_IC(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  if (funct3 != 0b000) {
    throw "Invalid function!";
  }
  return "jalr " + IntToString(rd, 5) + " " + IntToString(rs1, 5) + " " + IntToString(imm, 12);
}

std::string InstructionDecoder::Decode_IO(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20; 
  if (funct3 != 0b000) {
    throw "Invalid function!";
  }
  if (imm == 0) {
    return "ebreak";
  }
  if (imm == 1) {
    return "ecall";
  }
  throw "Invalid function!";
}

std::string InstructionDecoder::Decode_S(const int32_t code) {
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 25) & 0b1111111) << 5) | ((code >> 7) & 0b11111)) << 20) >> 20;
  switch (funct3) {
  case 0b000:
    return "sb " + IntToString(rs2, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  case 0b001:
    return "sh " + IntToString(rs2, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  case 0b010:
    return "sw " + IntToString(rs2, 5) + " " + IntToString(imm, 12) + "(" + IntToString(rs1, 5) + ")";
  default:
    throw "Invalid function!";
  }
}

std::string InstructionDecoder::Decode_B(const int32_t code) {
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 12) | (((code >> 7) & 0b1) << 11) | (((code >> 25) & 0b111111) << 5) | (((code >> 8) & 0b1111) << 1)) << 19) >> 19;
  switch (funct3) {
  case 0b000:
    return "beq " + IntToString(rs1, 5) + " " + IntToString(rs2, 5) + " " + IntToString(imm, 13);
  case 0b101:
    return "bge " + IntToString(rs1, 5) + " " + IntToString(rs2, 5) + " " + IntToString(imm, 13);
  case 0b111:
    return "bgeu " + IntToString(rs1, 5) + " " + IntToString(rs2, 5) + " " + IntToString(imm, 13);
  case 0b100:
    return "blt " + IntToString(rs1, 5) + " " + IntToString(rs2, 5) + " " + IntToString(imm, 13);
  case 0b110:
    return "bltu " + IntToString(rs1, 5) + " " + IntToString(rs2, 5) + " " + IntToString(imm, 13);
  case 0b001:
    return "bne " + IntToString(rs1, 5) + " " + IntToString(rs2, 5) + " " + IntToString(imm, 13);
  default:
    throw "Invalid function!";
  }
}

std::string InstructionDecoder::Decode_J(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 20) | (((code >> 12) & 0b11111111) << 12) | (((code >> 20) & 0b1) << 11) | (((code >> 21) & 0b1111111111) << 1)) << 11) >> 11;
  return "jal " + IntToString(rd, 5) + " " + IntToString(imm, 21);
}

std::string InstructionDecoder::Decode_AUIPC(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  return "auipc " + IntToString(rd, 5) + " " + IntToString(imm, 32);
}

std::string InstructionDecoder::Decode_LUI(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  return "lui " + IntToString(rd, 5) + " " + IntToString(imm, 32);
}

std::string InstructionDecoder::Decode(const int32_t code) {
  const int32_t opcode = code & 0b1111111;
  switch (opcode) {
  case 0b0110011:
    return Decode_R(code);
  case 0b0100011:
    return Decode_S(code);
  case 0b1100011:
    return Decode_B(code);
  case 0b1101111:
    return Decode_J(code);
  case 0b0010111:
    return Decode_AUIPC(code);
  case 0b0110111:
    return Decode_LUI(code);
  case 0b0010011:
    return Decode_IA(code);
  case 0b0000011:
    return Decode_IM(code);
  case 0b1100111:
    return Decode_IC(code);
  case 0b1110011:
    return Decode_IO(code);
  default:
    throw "No type matches!";
  }
}
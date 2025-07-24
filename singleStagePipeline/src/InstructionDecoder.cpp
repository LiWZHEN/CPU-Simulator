#include "../include/InstructionDecoder.hpp"
#include "Exception.hpp"
#include <iostream>

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

std::string InstructionDecoder::Decode_R(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      funct7 = (code >> 25) & 0b1111111;
  switch (funct3) {
  case 0b000:
    if (funct7 == 0b0000000) {
      return "add " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
    } else if (funct7 == 0b0100000) {
      return "sub " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
    } else {
      throw InvalidFunction();
    }
  case 0b111:
    return "and " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
  case 0b110:
    return "or " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
  case 0b100:
    return "xor " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
  case 0b001:
    return "sll " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
  case 0b101:
    if (funct7 == 0b0000000) {
      return "srl " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
    } else if (funct7 == 0b0100000) {
      return "sra " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
    } else {
      throw InvalidFunction();
    }
  case 0b010:
    return "slt " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
  case 0b011:
    return "sltu " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(rs2);
  default:
    throw InvalidFunction();
  }
}

std::string InstructionDecoder::Decode_IA(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111;
  if (funct3 == 0b001) {
    const int32_t imm = (code >> 20) & 0b11111,
        funct7 = (code >> 25) & 0b1111111;
    return "slli " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
  } else if (funct3 == 0b101) {
    const int32_t imm = (code >> 20) & 0b11111,
        funct7 = (code >> 25) & 0b1111111;
    if (funct7 == 0b0000000) {
      return "srli " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    } else if (funct7 == 0b0100000) {
      return "srai " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    } else {
      throw InvalidFunction();
    }
  } else {
    const int32_t imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
    switch (funct3) {
    case 0b000:
      return "addi " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    case 0b111:
      return "andi " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    case 0b110:
      return "ori " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    case 0b100:
      return "xori " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    case 0b010:
      return "slti " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    case 0b011:
      return "sltiu " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
    default:
      throw InvalidFunction();
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
    return "lb " + IntToString(rd) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  case 0b100:
    return "lbu " + IntToString(rd) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  case 0b001:
    return "lh " + IntToString(rd) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  case 0b101:
    return "lhu " + IntToString(rd) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  case 0b010:
    return "lw " + IntToString(rd) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  default:
    throw InvalidFunction();
  }
}

std::string InstructionDecoder::Decode_IC(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20;
  if (funct3 != 0b000) {
    throw InvalidFunction();
  }
  return "jalr " + IntToString(rd) + " " + IntToString(rs1) + " " + IntToString(imm);
}

std::string InstructionDecoder::Decode_IO(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      imm = (((code >> 20) & 0b111111111111) << 20) >> 20; 
  if (funct3 != 0b000) {
    throw InvalidFunction();
  }
  if (imm == 0) {
    return "ebreak";
  }
  if (imm == 1) {
    return "ecall";
  }
  throw InvalidFunction();
}

std::string InstructionDecoder::Decode_S(const int32_t code) {
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 25) & 0b1111111) << 5) | ((code >> 7) & 0b11111)) << 20) >> 20;
  switch (funct3) {
  case 0b000:
    return "sb " + IntToString(rs2) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  case 0b001:
    return "sh " + IntToString(rs2) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  case 0b010:
    return "sw " + IntToString(rs2) + " " + IntToString(imm) + "(" + IntToString(rs1) + ")";
  default:
    throw InvalidFunction();
  }
}

std::string InstructionDecoder::Decode_B(const int32_t code) {
  const int32_t funct3 = (code >> 12) & 0b111,
      rs1 = (code >> 15) & 0b11111,
      rs2 = (code >> 20) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 12) | (((code >> 7) & 0b1) << 11) | (((code >> 25) & 0b111111) << 5) | (((code >> 8) & 0b1111) << 1)) << 19) >> 19;
  switch (funct3) {
  case 0b000:
    return "beq " + IntToString(rs1) + " " + IntToString(rs2) + " " + IntToString(imm);
  case 0b101:
    return "bge " + IntToString(rs1) + " " + IntToString(rs2) + " " + IntToString(imm);
  case 0b111:
    return "bgeu " + IntToString(rs1) + " " + IntToString(rs2) + " " + IntToString(imm);
  case 0b100:
    return "blt " + IntToString(rs1) + " " + IntToString(rs2) + " " + IntToString(imm);
  case 0b110:
    return "bltu " + IntToString(rs1) + " " + IntToString(rs2) + " " + IntToString(imm);
  case 0b001:
    return "bne " + IntToString(rs1) + " " + IntToString(rs2) + " " + IntToString(imm);
  default:
    throw InvalidFunction();
  }
}

std::string InstructionDecoder::Decode_J(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      imm = (((((code >> 31) & 0b1) << 20) | (((code >> 12) & 0b11111111) << 12) | (((code >> 20) & 0b1) << 11) | (((code >> 21) & 0b1111111111) << 1)) << 11) >> 11;
  return "jal " + IntToString(rd) + " " + IntToString(imm);
}

std::string InstructionDecoder::Decode_AUIPC(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  return "auipc " + IntToString(rd) + " " + IntToString(imm);
}

std::string InstructionDecoder::Decode_LUI(const int32_t code) {
  const int32_t rd = (code >> 7) & 0b11111,
      imm = code & 0b11111111111111111111000000000000;
  return "lui " + IntToString(rd) + " " + IntToString(imm);
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
    throw NoMatchedType();
  }
}
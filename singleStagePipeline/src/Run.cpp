#include "../include/Run.hpp"
#include <iomanip>

void Interpreter::Add(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Add(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Sub(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Sub(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::And(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.And(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Or(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Or(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Xor(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Xor(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Sll(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Sll(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Srl(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Srl(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Sra(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Sra(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Slt(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const int32_t data1 = reg_file.GetData(rs1);
  const int32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Slt(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Sltu(const int32_t rd, const int32_t rs1, const int32_t rs2) {
  const uint32_t data1 = reg_file.GetData(rs1);
  const uint32_t data2 = reg_file.GetData(rs2);
  const int32_t result = ALU.Sltu(data1, data2);
  reg_file.Modify(rd, result);
}

void Interpreter::Slli(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Sll(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Srli(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Srl(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Srai(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Sra(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Addi(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Add(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Andi(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.And(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Ori(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Or(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Xori(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Xor(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Slti(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Slt(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Sltiu(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t result = ALU.Sltu(data_r, imm);
  reg_file.Modify(rd, result);
}

void Interpreter::Lb(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t address = ALU.Add(data_r, imm);
  int32_t data_m = memory.Get(address);
  data_m = (data_m << 24) >> 24;
  reg_file.Modify(rd, data_m);
}

void Interpreter::Lbu(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t address = ALU.Add(data_r, imm);
  const int32_t data_m = memory.Get(address);
  reg_file.Modify(rd, data_m);
}

void Interpreter::Lh(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t address = ALU.Add(data_r, imm);
  int32_t data_m = (memory.Get(address) | (memory.Get(address + 1) << 8));
  data_m = (data_m << 12) >> 12;
  reg_file.Modify(rd, data_m);
}

void Interpreter::Lhu(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t address = ALU.Add(data_r, imm);
  const int32_t data_m = (memory.Get(address) | (memory.Get(address + 1) << 8));
  reg_file.Modify(rd, data_m);
}

void Interpreter::Lw(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t data_r = reg_file.GetData(rs1);
  const int32_t address = ALU.Add(data_r, imm);
  const int32_t data_m = (memory.Get(address) | (memory.Get(address + 1) << 8) | (memory.Get(address + 2) << 16) | (memory.Get(address + 3) << 24));
  reg_file.Modify(rd, data_m);
}

void Interpreter::Jalr(const int32_t rd, const int32_t rs1, const int32_t imm) {
  const int32_t current_pc = pc.GetPC();
  reg_file.Modify(rd, ALU.Add(current_pc, 4));
  const int32_t new_pc = ALU.Add(reg_file.GetData(rs1), imm);
  pc.SetPC(new_pc);
}

void Interpreter::Ebreak() {
  std::cerr << "Ebreak function is called.\n";
}

void Interpreter::Ecall() {
  std::cerr << "Ecall function is called.\n";
}

void Interpreter::Sb(const int32_t rs2, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t address = ALU.Add(reg_file.GetData(rs1), imm);
  const uint8_t data = reg_file.GetData(rs2) & 0xFF;
  memory.Add(address, data);
}

void Interpreter::Sh(const int32_t rs2, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t address = ALU.Add(reg_file.GetData(rs1), imm);
  const int32_t data = reg_file.GetData(rs2);
  memory.Add(address, data & 0xFF);
  memory.Add(address + 1, (data >> 8) & 0xFF);
}

void Interpreter::Sw(const int32_t rs2, const int32_t imm, const int32_t rs1, Memory &memory) {
  const int32_t address = ALU.Add(reg_file.GetData(rs1), imm);
  const int32_t data = reg_file.GetData(rs2);
  memory.Add(address, data & 0xFF);
  memory.Add(address + 1, (data >> 8) & 0xFF);
  memory.Add(address + 2, (data >> 16) & 0xFF);
  memory.Add(address + 3, (data >> 24) & 0xFF);
}

void Interpreter::Beq(const int32_t rs1, const int32_t rs2, const int32_t imm) {
  if (branch_compare.Equal(reg_file.GetData(rs1), reg_file.GetData(rs2))) {
    const int32_t new_pc = ALU.Add(pc.GetPC(), imm);
    pc.SetPC(new_pc);
  } else {
    pc.NextPC();
  }
}

void Interpreter::Bge(const int32_t rs1, const int32_t rs2, const int32_t imm) {
  if (branch_compare.SignedGreaterOrEqual(reg_file.GetData(rs1), reg_file.GetData(rs2))) {
    const int32_t new_pc = ALU.Add(pc.GetPC(), imm);
    pc.SetPC(new_pc);
  } else {
    pc.NextPC();
  }
}

void Interpreter::Bgeu(const int32_t rs1, const int32_t rs2, const int32_t imm) {
  if (branch_compare.UnsignedGreaterOrEqual(reg_file.GetData(rs1), reg_file.GetData(rs2))) {
    const int32_t new_pc = ALU.Add(pc.GetPC(), imm);
    pc.SetPC(new_pc);
  } else {
    pc.NextPC();
  }
}

void Interpreter::Blt(const int32_t rs1, const int32_t rs2, const int32_t imm) {
  if (branch_compare.SignedLess(reg_file.GetData(rs1), reg_file.GetData(rs2))) {
    const int32_t new_pc = ALU.Add(pc.GetPC(), imm);
    pc.SetPC(new_pc);
  } else {
    pc.NextPC();
  }
}

void Interpreter::Bltu(const int32_t rs1, const int32_t rs2, const int32_t imm) {
  if (branch_compare.UnsignedLess(reg_file.GetData(rs1), reg_file.GetData(rs2))) {
    const int32_t new_pc = ALU.Add(pc.GetPC(), imm);
    pc.SetPC(new_pc);
  } else {
    pc.NextPC();
  }
}

void Interpreter::Bne(const int32_t rs1, const int32_t rs2, const int32_t imm) {
  if (branch_compare.NotEqual(reg_file.GetData(rs1), reg_file.GetData(rs2))) {
    const int32_t new_pc = ALU.Add(pc.GetPC(), imm);
    pc.SetPC(new_pc);
  } else {
    pc.NextPC();
  }
}

void Interpreter::Jal(const int32_t rd, const int32_t imm) {
  const int32_t current_pc = pc.GetPC();
  reg_file.Modify(rd, ALU.Add(current_pc, 4));
  const int32_t new_pc = ALU.Add(current_pc, imm);
  pc.SetPC(new_pc);
}

void Interpreter::Auipc(const int32_t rd, const int32_t imm) {
  const int32_t current_pc = pc.GetPC();
  reg_file.Modify(rd, ALU.Add(current_pc, imm));
}

void Interpreter::Lui(const int32_t rd, const int32_t imm) {
  reg_file.Modify(rd, imm);
}

void Interpreter::Run(Memory &memory) {
  while (true) {
    int32_t current_pc = pc.GetPC();
    if (memory.IsEmpty(current_pc)) {
      throw Nodata();
    }
    int32_t instruction = (memory.Get(current_pc) | (memory.Get(current_pc + 1) << 8) | (memory.Get(current_pc + 2) << 16) | (memory.Get(current_pc + 3) << 24));
    if (instruction == 0x0ff00513) {
      int32_t return_value = reg_file.GetData(10);
      std::cerr << "Exit\n";
      std::cout << (return_value & 0x000000FF) << '\n';
      return;
    }
    std::string command = decoder.Decode(instruction);
    std::cerr << std::hex << std::setw(6) << current_pc << " : " << std::setw(10) << instruction << " " << std::setw(36) << command << '\n';
    if (command.substr(0, 4) == "add ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Add(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "sub ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Sub(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "and ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      And(rd, rs1, rs2);
    } else if (command.substr(0, 3) == "or ") {
      const int32_t rd = StringToInt(command.substr(3, 8));
      const int32_t rs1 = StringToInt(command.substr(12, 8));
      const int32_t rs2 = StringToInt(command.substr(21, 8));
      Or(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "xor ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Xor(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "sll ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Sll(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "srl ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Srl(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "sra ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Sra(rd, rs1, rs2);
    } else if (command.substr(0, 4) == "slt ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t rs2 = StringToInt(command.substr(22, 8));
      Slt(rd, rs1, rs2);
    } else if (command.substr(0, 5) == "sltu ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t rs2 = StringToInt(command.substr(23, 8));
      Sltu(rd, rs1, rs2);
    } else if (command.substr(0, 5) == "slli ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Slli(rd, rs1, imm);
    } else if (command.substr(0, 5) == "srli ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Srli(rd, rs1, imm);
    } else if (command.substr(0, 5) == "srai ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Srai(rd, rs1, imm);
    } else if (command.substr(0, 5) == "addi ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Addi(rd, rs1, imm);
    } else if (command.substr(0, 5) == "andi ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Andi(rd, rs1, imm);
    } else if (command.substr(0, 4) == "ori ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t rs1 = StringToInt(command.substr(13, 8));
      const int32_t imm = StringToInt(command.substr(22, 8));
      Ori(rd, rs1, imm);
    } else if (command.substr(0, 5) == "xori ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Xori(rd, rs1, imm);
    } else if (command.substr(0, 5) == "slti ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Slti(rd, rs1, imm);
    } else if (command.substr(0, 6) == "sltiu ") {
      const int32_t rd = StringToInt(command.substr(6, 8));
      const int32_t rs1 = StringToInt(command.substr(15, 8));
      const int32_t imm = StringToInt(command.substr(24, 8));
      Sltiu(rd, rs1, imm);
    } else if (command.substr(0, 3) == "lb ") {
      const int32_t rd = StringToInt(command.substr(3, 8));
      const int32_t imm = StringToInt(command.substr(12, 8));
      const int32_t rs1 = StringToInt(command.substr(21, 8));
      Lb(rd, imm, rs1, memory);
    } else if (command.substr(0, 4) == "lbu ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t imm = StringToInt(command.substr(13, 8));
      const int32_t rs1 = StringToInt(command.substr(22, 8));
      Lbu(rd, imm, rs1, memory);
    } else if (command.substr(0, 3) == "lh ") {
      const int32_t rd = StringToInt(command.substr(3, 8));
      const int32_t imm = StringToInt(command.substr(12, 8));
      const int32_t rs1 = StringToInt(command.substr(21, 8));
      Lh(rd, imm, rs1, memory);
    } else if (command.substr(0, 4) == "lhu ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t imm = StringToInt(command.substr(13, 8));
      const int32_t rs1 = StringToInt(command.substr(22, 8));
      Lhu(rd, imm, rs1, memory);
    } else if (command.substr(0, 3) == "lw ") {
      const int32_t rd = StringToInt(command.substr(3, 8));
      const int32_t imm = StringToInt(command.substr(12, 8));
      const int32_t rs1 = StringToInt(command.substr(21, 8));
      Lw(rd, imm, rs1, memory);
    } else if (command.substr(0, 5) == "jalr ") {
      const int32_t rd = StringToInt(command.substr(5, 8));
      const int32_t rs1 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Jalr(rd, rs1, imm);
      continue;
    } else if (command == "ebreak") {
      Ebreak();
    } else if (command == "ecall") {
      Ecall();
    } else if (command.substr(0, 3) == "sb ") {
      const int32_t rs2 = StringToInt(command.substr(3, 8));
      const int32_t imm = StringToInt(command.substr(12, 8));
      const int32_t rs1 = StringToInt(command.substr(21, 8));
      Sb(rs2, imm, rs1, memory);
    } else if (command.substr(0, 3) == "sh ") {
      const int32_t rs2 = StringToInt(command.substr(3, 8));
      const int32_t imm = StringToInt(command.substr(12, 8));
      const int32_t rs1 = StringToInt(command.substr(21, 8));
      Sh(rs2, imm, rs1, memory);
    } else if (command.substr(0, 3) == "sw ") {
      const int32_t rs2 = StringToInt(command.substr(3, 8));
      const int32_t imm = StringToInt(command.substr(12, 8));
      const int32_t rs1 = StringToInt(command.substr(21, 8));
      Sw(rs2, imm, rs1, memory);
    } else if (command.substr(0, 4) == "beq ") {
      const int32_t rs1 = StringToInt(command.substr(4, 8));
      const int32_t rs2 = StringToInt(command.substr(13, 8));
      const int32_t imm = StringToInt(command.substr(22, 8));
      Beq(rs1, rs2, imm);
      continue;
    } else if (command.substr(0, 4) == "bge ") {
      const int32_t rs1 = StringToInt(command.substr(4, 8));
      const int32_t rs2 = StringToInt(command.substr(13, 8));
      const int32_t imm = StringToInt(command.substr(22, 8));
      Bge(rs1, rs2, imm);
      continue;
    } else if (command.substr(0, 5) == "bgeu ") {
      const int32_t rs1 = StringToInt(command.substr(5, 8));
      const int32_t rs2 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Bgeu(rs1, rs2, imm);
      continue;
    } else if (command.substr(0, 4) == "blt ") {
      const int32_t rs1 = StringToInt(command.substr(4, 8));
      const int32_t rs2 = StringToInt(command.substr(13, 8));
      const int32_t imm = StringToInt(command.substr(22, 8));
      Blt(rs1, rs2, imm);
      continue;
    } else if (command.substr(0, 5) == "bltu ") {
      const int32_t rs1 = StringToInt(command.substr(5, 8));
      const int32_t rs2 = StringToInt(command.substr(14, 8));
      const int32_t imm = StringToInt(command.substr(23, 8));
      Bltu(rs1, rs2, imm);
      continue;
    } else if (command.substr(0, 4) == "bne ") {
      const int32_t rs1 = StringToInt(command.substr(4, 8));
      const int32_t rs2 = StringToInt(command.substr(13, 8));
      const int32_t imm = StringToInt(command.substr(22, 8));
      Bne(rs1, rs2, imm);
      continue;
    } else if (command.substr(0, 4) == "jal ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t imm = StringToInt(command.substr(13, 8));
      Jal(rd, imm);
      continue;
    } else if (command.substr(0, 6) == "auipc ") {
      const int32_t rd = StringToInt(command.substr(6, 8));
      const int32_t imm = StringToInt(command.substr(15, 8));
      Auipc(rd, imm);
      continue;
    } else if (command.substr(0, 4) == "lui ") {
      const int32_t rd = StringToInt(command.substr(4, 8));
      const int32_t imm = StringToInt(command.substr(13, 8));
      Lui(rd, imm);
    } else {
      throw InvalidInstruction();
    }
    pc.NextPC();
  }
}
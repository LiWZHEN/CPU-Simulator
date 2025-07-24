#ifndef RUN_HPP
#define RUN_HPP

#include "Exception.hpp"
#include "ProgramCounter.hpp"
#include "Memory.hpp"
#include "InstructionDecoder.hpp"
#include "RegFile.hpp"
#include "ArithmeticLogicUnit.hpp"
#include "BranchComparation.hpp"

class Interpreter {
private:
  RegFile reg_file;
  ProgramCounter pc;
  InstructionDecoder decoder;
  ArithmeticLogicUnit ALU;
  BranchComparation branch_compare;
public:
  Interpreter() = default;
  void Run(Memory &memory);
private:
  void Add(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Sub(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void And(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Or(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Xor(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Sll(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Srl(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Sra(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Slt(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Sltu(const int32_t rd, const int32_t rs1, const int32_t rs2);
  void Slli(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Srli(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Srai(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Addi(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Andi(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Ori(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Xori(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Slti(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Sltiu(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Lb(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory);
  void Lbu(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory);
  void Lh(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory);
  void Lhu(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory);
  void Lw(const int32_t rd, const int32_t imm, const int32_t rs1, Memory &memory);
  void Jalr(const int32_t rd, const int32_t rs1, const int32_t imm);
  void Ebreak();
  void Ecall();
  void Sb(const int32_t rs2, const int32_t imm, const int32_t rs1, Memory &memory);
  void Sh(const int32_t rs2, const int32_t imm, const int32_t rs1, Memory &memory);
  void Sw(const int32_t rs2, const int32_t imm, const int32_t rs1, Memory &memory);
  void Beq(const int32_t rs1, const int32_t rs2, const int32_t imm);
  void Bge(const int32_t rs1, const int32_t rs2, const int32_t imm);
  void Bgeu(const int32_t rs1, const int32_t rs2, const int32_t imm);
  void Blt(const int32_t rs1, const int32_t rs2, const int32_t imm);
  void Bltu(const int32_t rs1, const int32_t rs2, const int32_t imm);
  void Bne(const int32_t rs1, const int32_t rs2, const int32_t imm);
  void Jal(const int32_t rd, const int32_t imm);
  void Auipc(const int32_t rd, const int32_t imm);
  void Lui(const int32_t rd, const int32_t imm);
};

#endif
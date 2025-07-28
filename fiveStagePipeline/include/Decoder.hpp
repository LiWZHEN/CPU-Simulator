#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <string>
#include "ReorderBuffer.hpp"
#include "ReservationStation.hpp"
#include "LoadStoreBuffer.hpp"
#include "RegisterFile.hpp"

enum instruction_type {ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT, SLTU,
    ADDI, ANDI, ORI, XORI, SLLI, SRLI, SRAI, SLTI, SLTIU, LB, LBU, LH,
    LHU, LW, SB, SH, SW, BEQ, BGE, BGEU, BLT, BLTU, BNE, JAL, JALR,
    AUIPC, LUI, EBREAK, ECALL, MUL, EXIT};

struct DecoderTask {
  int32_t machine_code;
  instruction_type type;
  int32_t imm;
  int32_t rs1 = -1, rs2 = -1;
  int32_t V1 = 0, V2 = 0;
  int32_t Q1 = -1, Q2 = -1;
  int32_t destination = -1;
};

class Decoder {
  DecoderTask old_task, new_task;
  ROB *rob = nullptr;
  RS *rs = nullptr;
  LSB *lsb = nullptr;
  RegisterFile *register_file = nullptr;

  void Decode_R();
  void Decode_IA();
  void Decode_IM();
  void Decode_IC();
  void Decode_IO();
  void Decode_S();
  void Decode_B();
  void Decode_J();
  void Decode_AUIPC();
  void Decode_LUI();
public:
  Decoder() = default;
  void Connect(ROB *rob, RS *rs, LSB *lsb, RegisterFile *register_file);
  void SetTask(int32_t machine_code);
  void Update();
  void Run();
};

std::string IntToString(uint32_t x);
uint32_t StringToInt(const std::string &str);

#endif
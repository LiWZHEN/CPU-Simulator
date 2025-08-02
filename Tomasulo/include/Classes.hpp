#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <iostream>

class ALU;
class CPU;
class Decoder;
class DataLoader;
class LSB;
class Memory;
class Predictor;
class ProgramCounter;
class RegisterFile;
class ROB;
class RS;


enum InstructionType {NONE, ADD, SUB, AND, OR, XOR, SLL, SRL, SRA, SLT,
    SLTU, ADDI, ANDI, ORI, XORI, SLLI, SRLI, SRAI, SLTI, SLTIU, LB, LBU,
    LH, LHU, LW, SB, SH, SW, BEQ, BGE, BGEU, BLT, BLTU, BNE, JAL, JALR,
    AUIPC, LUI, EBREAK, ECALL, MUL, EXIT};

std::string PrintType(InstructionType type);

#endif
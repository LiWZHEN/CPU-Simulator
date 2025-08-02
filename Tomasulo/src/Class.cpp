#include "../include/Classes.hpp"

std::string PrintType(InstructionType type) {
  switch (type) {
  case 0:
    return "None";
    
  case 1:
    return "Add";
    
  case 2:
    return "Sub";
    
  case 3:
    return "And";
    
  case 4:
    return "Or";
    
  case 5:
    return "Xor";
    
  case 6:
    return "sll";
    
  case 7:
    return "srl";
    
  case 8:
    return "Sra";
    
  case 9:
    return "Slt";
    
  case 10:
    return "Sltu";
    
  case 11:
    return "Addi";
    
  case 12:
    return "Andi";
    
  case 13:
    return "Ori";
    
  case 14:
    return "Xori";
    
  case 15:
    return "slli";
    
  case 16:
    return "Srli";
    
  case 17:
    return "Srai";
    
  case 18:
    return "Slti";
    
  case 19:
    return "Sltiu";
    
  case 20:
    return "Lb";
    
  case 21:
    return "Lbu";
    
  case 22:
    return "Lh";
    
  case 23:
    return "Lhu";
    
  case 24:
    return "Lw";
    
  case 25:
    return "Sb";
    
  case 26:
    return "Sh";
    
  case 27:
    return "Sw";
    
  case 28:
    return "Beq";
    
  case 29:
    return "Bge";
    
  case 30:
    return "Bgeu";
    
  case 31:
    return "Blt";
    
  case 32:
    return "Bltu";
    
  case 33:
    return "Bne";
    
  case 34:
    return "Jal";
    
  case 35:
    return "Jalr";
    
  case 36:
    return "Auipc";
    
  case 37:
    return "Lui";
    
  case 38:
    return "Ebreak";
    
  case 39:
    return "Ecall";
    
  case 40:
    return "Mul";
    
  case 41:
    return "Exit";
    
  
  default:
    return "ERROR!!!";
  }
}
#ifndef ARITHMETIC_LOGIC_UNIT_HPP
#define ARITHMETIC_LOGIC_UNIT_HPP

#include <cstdint>

class ArithmeticLogicUnit {
public:
  int32_t Add(const int32_t n1, const int32_t n2);
  int32_t Sub(const int32_t n1, const int32_t n2);
  int32_t Not(const int32_t n);
  int32_t And(const int32_t n1, const int32_t n2);
  int32_t Or(const int32_t n1, const int32_t n2);
  int32_t Xor(const int32_t n1, const int32_t n2);
  int32_t Sll(const int32_t n1, const int32_t n2);
  int32_t Srl(const int32_t n1, const int32_t n2);
  int32_t Sra(const int32_t n1, const int32_t n2);
};
#endif
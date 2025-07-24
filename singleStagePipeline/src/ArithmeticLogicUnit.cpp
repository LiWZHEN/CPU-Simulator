#include "../include/ArithmeticLogicUnit.hpp"

int32_t ArithmeticLogicUnit::Add(const int32_t n1, const int32_t n2) {
  return n1 + n2;
}

int32_t ArithmeticLogicUnit::Sub(const int32_t n1, const int32_t n2) {
  return n1 - n2;
}

int32_t ArithmeticLogicUnit::Not(const int32_t n) {
  return ~n;
}

int32_t ArithmeticLogicUnit::And(const int32_t n1, const int32_t n2) {
  return n1 & n2;
}

int32_t ArithmeticLogicUnit::Or(const int32_t n1, const int32_t n2) {
  return n1 | n2;
}

int32_t ArithmeticLogicUnit::Xor(const int32_t n1, const int32_t n2) {
  return n1 ^ n2;
}

int32_t ArithmeticLogicUnit::Sll(const int32_t n1, const int32_t n2) {
  return n1 << n2;
}

int32_t ArithmeticLogicUnit::Srl(const int32_t n1, const int32_t n2) {
  const uint32_t un1 = n1;
  return un1 >> n2;
}

int32_t ArithmeticLogicUnit::Sra(const int32_t n1, const int32_t n2) {
  return n1 >> n2;
}

int32_t ArithmeticLogicUnit::Slt(const int32_t n1, const int32_t n2) {
  return (n1 < n2) ? 1 : 0;
}

int32_t ArithmeticLogicUnit::Sltu(const uint32_t n1, const uint32_t n2) {
  return (n1 < n2) ? 1 : 0;
}
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
  if (n1 & 0b10000000000000000000000000000000 == 0) {
    return n1 >> n2;
  }
  if (n2 == 0) {
    return n1;
  }
  return ((n1 >> 1) | 0b01111111111111111111111111111111) >> (n2 - 1);
}

int32_t ArithmeticLogicUnit::Sra(const int32_t n1, const int32_t n2) {
  return n1 >> n2;
}
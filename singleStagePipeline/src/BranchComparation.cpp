#include "../include/BranchComparation.hpp"

bool BranchComparation::Equal(const int32_t rs1, const int32_t rs2) {
  return rs1 == rs2;
}

bool BranchComparation::SignedGreaterOrEqual(const int32_t rs1, const int32_t rs2) {
  return rs1 >= rs2;
}

bool BranchComparation::UnsignedGreaterOrEqual(const int32_t rs1, const int32_t rs2) {
  const uint32_t urs1 = rs1, urs2 = rs2;
  return rs1 >= rs2;
}

bool BranchComparation::SignedLess(const int32_t rs1, const int32_t rs2) {
  return rs1 < rs2;
}

bool BranchComparation::UnsignedLess(const int32_t rs1, const int32_t rs2) {
  const uint32_t urs1 = rs1, urs2 = rs2;
  return rs1 < rs2;
}

bool BranchComparation::NotEqual(const int32_t rs1, const int32_t rs2) {
  return rs1 != rs2;
}
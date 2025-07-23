#include "../include/BranchComparation.hpp"

bool BranchComparation::Equal(const int32_t rs1, const int32_t rs2) {
  return rs1 == rs2;
}

bool BranchComparation::SignedGreaterOrEqual(const int32_t rs1, const int32_t rs2) {
  return rs1 >= rs2;
}

bool BranchComparation::UnsignedGreaterOrEqual(const int32_t rs1, const int32_t rs2) {
  for (int i = 31; i >= 0; --i) {
    if ((rs1 & (1 << i)) == 0 && (rs1 & (1 << i)) != 0) {
      return false;
    }
  }
  return true;
}

bool BranchComparation::SignedLess(const int32_t rs1, const int32_t rs2) {
  return rs1 < rs2;
}

bool BranchComparation::UnsignedLess(const int32_t rs1, const int32_t rs2) {
  return !UnsignedGreaterOrEqual(rs1, rs2);
}

bool BranchComparation::NotEqual(const int32_t rs1, const int32_t rs2) {
  return rs1 != rs2;
}
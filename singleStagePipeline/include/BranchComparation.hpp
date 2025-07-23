#ifndef BRANCH_COMPARATION_HPP
#define BRANCH_COMPARATION_HPP

#include <cstdint>

class BranchComparation {
public:
  bool Equal(const int32_t rs1, const int32_t rs2);
  bool SignedGreaterOrEqual(const int32_t rs1, const int32_t rs2);
  bool UnsignedGreaterOrEqual(const int32_t rs1, const int32_t rs2);
  bool SignedLess(const int32_t rs1, const int32_t rs2);
  bool UnsignedLess(const int32_t rs1, const int32_t rs2);
  bool NotEqual(const int32_t rs1, const int32_t rs2);
};

#endif
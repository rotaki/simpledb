#pragma once

#include "plan/plan.hpp"
#include "query/expression.hpp"

namespace simpledb {
class term {
public:
  term();
  term(const term &pT);
  term(const expression &pLhs, const expression &pRhs);
  term &operator=(const term &pT);
  bool is_satisfied(scan *pS) const;
  bool applies_to(const schema &pSch) const;
  int reduction_factor(plan *pPlan) const;
  constant equates_with_constant(const std::string &pFldName) const;
  std::string equates_with_field(const std::string &pFldName) const;
  std::string to_string() const;

private:
  expression mLhs, mRhs;
};
} // namespace simpledb

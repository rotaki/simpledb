#pragma once

#include "expression.hpp"
#include "plan.hpp"

namespace smartdb {
  class term {
  public:
    term();
    term(const term &pT);
    term(const expression &pLhs, const expression &pRhs);
    term& operator=(const term &pT);
    bool is_satisfied(std::shared_ptr<scan> pS) const;
    bool applies_to(std::shared_ptr<schema> pSch) const;
    int reduction_factor(std::shared_ptr<plan> pPlan) const;
    constant equates_with_constant(const std::string &pFldName) const;
    std::string equates_with_field(const std::string &pFldName) const;
    std::string to_string() const;
  private:
    expression mLhs, mRhs;
  };
}

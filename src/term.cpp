#include <limits>

#include "term.hpp"

namespace smartdb {
  term::term() {}
  
  term::term(const term &pT):
    mLhs(pT.mLhs), mRhs(pT.mRhs) {}

  term::term(const expression &pLhs, const expression &pRhs):
    mLhs(pLhs), mRhs(pRhs) {}

  term& term::operator=(const term &pT) {
    if (this != &pT) {
      mLhs = pT.mLhs;
      mRhs = pT.mRhs;
    }
    return *this;
  }

  bool term::is_satisfied(std::shared_ptr<scan> pS) const {
    constant lhsVal = mLhs.evaluate(pS);
    constant rhsVal = mRhs.evaluate(pS);
    return rhsVal == lhsVal;
  }

  bool term::applies_to(std::shared_ptr<schema> pSch) const {
    return mLhs.applies_to(pSch) && mRhs.applies_to(pSch);
  }

  int term::reduction_factor(std::shared_ptr<plan> pPlan) const {
    std::string lhsName, rhsName;
    if (mLhs.is_field_name() && mRhs.is_field_name()) {
      lhsName = mLhs.as_field_name();
      rhsName = mRhs.as_field_name();
      return std::max(pPlan->distinct_values(lhsName), pPlan->distinct_values(rhsName));
    }
    if (mLhs.is_field_name()) {
      lhsName = mLhs.as_field_name();
      return pPlan->distinct_values(lhsName);
    }
    if (mRhs.is_field_name()) {
      rhsName = mRhs.as_field_name();
      return pPlan->distinct_values(rhsName);
    }
    // otherwise, the term equates constants
    if (mLhs.as_constant() == mRhs.as_constant()) {
      return 1;
    } else {
      return std::numeric_limits<int>::max();
    }
  }

  constant term::equates_with_constant(const std::string &pFldName) const {
    if (mLhs.is_field_name() && mLhs.as_field_name() == pFldName && !mRhs.is_field_name()) {
      return mRhs.as_constant();
    } else if (mRhs.is_field_name() && mRhs.as_field_name() == pFldName && !mLhs.is_field_name()) {
      return mLhs.as_constant();
    } else {
      constant c;
      return c;
    }
  }

  std::string term::equates_with_field(const std::string &pFldName) const {
    if (mLhs.is_field_name() && mLhs.as_field_name() == pFldName && mRhs.is_field_name()) {
      return mRhs.as_field_name();
    } else if (mRhs.is_field_name() && mRhs.as_field_name() == pFldName && mLhs.is_field_name()) {
      return mLhs.as_field_name();
    } else {
      return "";
    }
  }

  std::string term::to_string() const {
    return mLhs.to_string() + "=" + mRhs.to_string();
  }
}

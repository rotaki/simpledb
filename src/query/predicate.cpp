#include <iostream>
#include "query/predicate.hpp"

namespace simpledb {
  predicate::predicate(){};

  predicate::predicate(const predicate &pPred):
    mTerms(pPred.mTerms) {}
  
  predicate::predicate(const term &pT) {
    mTerms.emplace_back(pT);
  }

  predicate& predicate::operator=(const predicate &pPred) {
    if (this != &pPred) {
      mTerms = pPred.mTerms;
    }
    return *this;
  }

  bool predicate::is_null() const {
    return (mTerms.size() == 0);
  }

  void predicate::con_join_with(const predicate &pP) {
    mTerms.insert(mTerms.end(), pP.mTerms.begin(), pP.mTerms.end());
  }

  bool predicate::is_satisfied(scan* pS) const {
    for (const term &t: mTerms) {
      if (!t.is_satisfied(pS)) {
        return false;
      }
    }
    return true;
  }

  int predicate::reduction_factor(plan* pPlan) const {
    int factor = 1;
    for (const term &t: mTerms) {
      factor *= t.reduction_factor(pPlan);
    }
    return factor;
  }


  predicate predicate::select_sub_pred(const schema &pSch) const {
    predicate result;
    for (const term &t: mTerms) {
      if (t.applies_to(pSch)) {
        result.mTerms.emplace_back(t);
      }
    }
    return result;
  }


  predicate predicate::join_sub_pred(const schema &pSch1, const schema &pSch2) const {
    predicate result;
    schema newSch;
    newSch.add_all(pSch1);
    newSch.add_all(pSch2);
    for (const term &t: mTerms) {
      if (!t.applies_to(pSch1) && !t.applies_to(pSch2) && t.applies_to(newSch)) {
        result.mTerms.emplace_back(t);
      }
    }
    return result;
  }

  constant predicate::equates_with_constant(const std::string &pFldName) const {
    for (const term &t: mTerms) {
      constant c = t.equates_with_constant(pFldName);
      if (!c.is_null()) {
        return c;
      }
    }
    constant n;
    return n;
  }

  std::string predicate::equates_with_field(const std::string &pFldName) const {
    for (const term &t: mTerms) {
      std::string s = t.equates_with_field(pFldName);
      if (!s.empty()) {
        return s;
      }
    }
    return "";
  }

  std::string predicate::to_string() const {
    std::string result = "";
    int cnt = 0;
    for (const term &t: mTerms) {
      cnt++;
      result += t.to_string();
      if (cnt < static_cast<int>(mTerms.size())) {
        result += " and ";
      }
    }
    return result;
  }
}

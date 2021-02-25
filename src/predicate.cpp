#include "predicate.hpp"

namespace smartdb {
  predicate::predicate(){};
  predicate::predicate(std::shared_ptr<term> pT) {
    mTerms.emplace_back(pT);
  }

  void predicate::con_join_with(std::shared_ptr<predicate> pP) {
    mTerms.insert(mTerms.end(), pP->mTerms.begin(), pP->mTerms.end());
  }

  bool predicate::is_satisfied(std::shared_ptr<scan> pS) {
    for (auto t: mTerms) {
      if (!t->is_satisfied(pS)) {
        return false;
      }
    }
    return true;
  }

  int predicate::reduction_factor(std::shared_ptr<plan> pPlan) {
    int factor = 1;
    for (auto t: mTerms) {
      factor *= t->reduction_factor(pPlan);
    }
    return factor;
  }


  std::shared_ptr<predicate> predicate::select_sub_pred(std::shared_ptr<schema> pSch){
    std::shared_ptr<predicate> result(new predicate);
    for (auto t: mTerms) {
      if (t->applies_to(pSch)) {
        result->mTerms.emplace_back(t);
      }
    }
    if (result->mTerms.size() == 0) {
      return std::shared_ptr<predicate>(nullptr);
    } else {
      return result;              
    }
  }


  std::shared_ptr<predicate> predicate::join_sub_pred(std::shared_ptr<schema> pSch1, std::shared_ptr<schema> pSch2) {
    std::shared_ptr<predicate> result(new predicate);
    std::shared_ptr<schema> newSch(new schema);
    newSch->add_all(pSch1);
    newSch->add_all(pSch2);
    for (auto t: mTerms) {
      if (!t->applies_to(pSch1) && !t->applies_to(pSch2) && t->applies_to(newSch)) {
        result->mTerms.emplace_back(t);
      }
    }
    if (result->mTerms.size() == 0) {
      return std::shared_ptr<predicate>(nullptr);
    } else {
      return result;              
    }
  }

  constant predicate::equates_with_constant(const std::string &pFldName) {
    for (auto t: mTerms) {
      constant c = t->equates_with_constant(pFldName);
      if (!c.is_null()) {
        return c;
      }
    }
    constant n;
    return n;
  }

  std::string predicate::equates_with_field(const std::string &pFldName) {
    for (auto t: mTerms) {
      std::string s = t->equates_with_field(pFldName);
      if (!s.empty()) {
        return s;
      }
    }
    return "";
  }

  std::string predicate::to_string() {
    std::string result = "";
    int cnt = 0;
    for (auto t: mTerms) {
      cnt++;
      result += t->to_string();
      if (cnt < static_cast<int>(mTerms.size())) {
        result += " and ";
      }
    }
    return result;
  }
}

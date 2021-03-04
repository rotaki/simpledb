#pragma once

#include <memory>
#include <vector>

#include "query/term.hpp"
#include "query/scan.hpp"
#include "record/schema.hpp"
#include "plan/plan.hpp"

namespace smartdb {
  class predicate {
  public:
    predicate();
    predicate(const predicate &pPred);
    predicate(const term &pT);
    predicate& operator=(const predicate &pPred);
    bool is_null() const;
    void con_join_with(const predicate &pP);
    bool is_satisfied(std::shared_ptr<scan> pS) const;
    int reduction_factor(std::shared_ptr<plan> pPlan) const;
    predicate select_sub_pred(std::shared_ptr<schema> pSch) const;
    predicate join_sub_pred(std::shared_ptr<schema> pSch1, std::shared_ptr<schema> pSch2) const;
    constant equates_with_constant(const std::string &pFldName) const;
    std::string equates_with_field(const std::string &pFldName) const;
    std::string to_string() const;
  private:
    std::vector<term> mTerms;
  };
}

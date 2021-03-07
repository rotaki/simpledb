#pragma once

#include <memory>

#include "plan/plan.hpp"
#include "query/predicate.hpp"

namespace smartdb {
  class select_plan: public plan {
  public:
    select_plan(const std::shared_ptr<plan> &pPlan, const predicate &pPred);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    schema get_schema() override;
  private:
    std::shared_ptr<plan> mPlan;
    predicate mPred;
  };
}

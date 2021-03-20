#pragma once

#include "plan/plan.hpp"
#include "query/updatescan.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class materialize_plan : public plan {
public:
  materialize_plan(transaction *pTx, const std::shared_ptr<plan> &pSrcPlan);
  std::shared_ptr<scan> open() override;
  int blocks_accessed() override;
  int records_output() override;
  int distinct_values(const std::string &pFldName) override;
  schema get_schema() override;

private:
  transaction *mTx;
  std::shared_ptr<plan> mSrcPlan;
};
} // namespace simpledb

#pragma once

#include "materialize/temptable.hpp"
#include "plan/plan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class multibuffer_product_plan : public plan {
public:
  multibuffer_product_plan(transaction &tx, const std::shared_ptr<plan> &pLhs,
                           const std::shared_ptr<plan> &pRhs);
  std::shared_ptr<scan> open() override;
  int blocks_accessed() override;
  int records_output() override;
  int distinct_values(const std::string &pFldName) override;
  schema get_schema() override;

private:
  transaction &mTx;
  std::shared_ptr<plan> mLhs, mRhs;
  schema mSch;

  std::shared_ptr<temp_table> copy_records_from(plan &p);
};
} // namespace simpledb

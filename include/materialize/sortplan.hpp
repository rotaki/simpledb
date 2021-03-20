#pragma once

#include "materialize/materializeplan.hpp"
#include "materialize/recordcomparator.hpp"
#include "materialize/sortscan.hpp"
#include "materialize/temptable.hpp"
#include "plan/plan.hpp"
#include "query/updatescan.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class sort_plan : public plan {
public:
  sort_plan(transaction *pTx, const std::shared_ptr<plan> &pP,
            const std::vector<std::string> &pSortFields);
  std::shared_ptr<scan> open() override;
  int blocks_accessed() override;
  int records_output() override;
  int distinct_values(const std::string &pFldName) override;
  schema get_schema() override;

private:
  std::shared_ptr<plan> mP;
  transaction *mTx;
  schema mSch;
  record_comparator mComp;

  std::vector<std::shared_ptr<temp_table>> split_into_runs(scan *pSrc);
  std::vector<std::shared_ptr<temp_table>>
  do_a_merge_iteration(const std::vector<std::shared_ptr<temp_table>> &pRuns);
  std::shared_ptr<temp_table> merge_two_runs(temp_table *pP1, temp_table *pP2);
  bool copy(scan *pSrc, update_scan *pDest);
};
} // namespace simpledb

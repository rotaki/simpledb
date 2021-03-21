#pragma once

#include <set>

#include "metadata/metadatamanager.hpp"
#include "opt/tableplanner.hpp"
#include "parse/querydata.hpp"
#include "plan/plan.hpp"
#include "plan/planner.hpp"
#include "plan/queryplanner.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class heuristic_query_planner : public query_planner {
public:
  heuristic_query_planner(metadata_manager &pMM);
  std::shared_ptr<plan> create_plan(query_data *pData,
                                    transaction *pTx) override;
  std::shared_ptr<plan> get_lowest_select_plan();
  std::shared_ptr<plan>
  get_lowest_join_plan(const std::shared_ptr<plan> &pCurrent);
  std::shared_ptr<plan>
  get_lowest_product_plan(const std::shared_ptr<plan> &pCurrent);
  void set_planner(planner *pP);

private:
  metadata_manager &mMM;
  std::set<std::shared_ptr<table_planner>> mTablePlanners;
};
} // namespace simpledb

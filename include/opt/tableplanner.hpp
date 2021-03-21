#pragma once

#include <map>
#include <string>

#include "metadata/indexmanager.hpp"
#include "metadata/metadatamanager.hpp"
#include "plan/plan.hpp"
#include "plan/tableplan.hpp"
#include "query/predicate.hpp"
#include "record/schema.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class table_planner {
public:
  table_planner(const std::string &pTblName, const predicate &pMyPred,
                transaction &pTx, metadata_manager &pMM);
  std::shared_ptr<plan> make_select_plan();
  std::shared_ptr<plan> make_join_plan(const std::shared_ptr<plan> &pCurrent);
  std::shared_ptr<plan>
  make_product_plan(const std::shared_ptr<plan> &pCurrent);

private:
  predicate mMyPred;
  transaction &mTx;
  std::shared_ptr<table_plan> mMyPlan;
  schema mMySchema;
  std::map<std::string, index_info> mIndexes;

  std::shared_ptr<plan> make_index_select();
  std::shared_ptr<plan> make_index_join(const std::shared_ptr<plan> &pCurrent,
                                        const schema &pCurrSch);
  std::shared_ptr<plan> make_product_join(const std::shared_ptr<plan> &pCurrent,
                                          const schema &pCurrSch);
  std::shared_ptr<plan> add_select_pred(const std::shared_ptr<plan> &pP);
  std::shared_ptr<plan> add_join_pred(const std::shared_ptr<plan> &pP,
                                      const schema &pCurrSch);
};
} // namespace simpledb

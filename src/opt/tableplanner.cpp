#include <iostream>

#include "index/planner/indexjoinplan.hpp"
#include "index/planner/indexselectplan.hpp"
#include "multibuffer/multibufferproductplan.hpp"
#include "opt/tableplanner.hpp"
#include "plan/selectplan.hpp"

namespace simpledb {
table_planner::table_planner(const std::string &pTblName,
                             const predicate &pMyPred, transaction &pTx,
                             metadata_manager &pMM)
    : mMyPred(pMyPred), mTx(pTx) {
  mMyPlan = std::make_shared<table_plan>(&mTx, pTblName, &pMM);
  mMySchema = mMyPlan->get_schema();
  mIndexes = pMM.get_index_info(pTblName, &mTx);
}

std::shared_ptr<plan> table_planner::make_select_plan() {
  std::shared_ptr<plan> p = make_index_select();
  if (!p) {
    p = mMyPlan;
  }
  return add_select_pred(p);
}

std::shared_ptr<plan>
table_planner::make_join_plan(const std::shared_ptr<plan> &pCurrent) {
  schema currSch = pCurrent->get_schema();
  predicate joinPred = mMyPred.join_sub_pred(mMySchema, currSch);
  if (joinPred.is_null()) {
    return nullptr;
  }
  std::shared_ptr<plan> p = make_index_join(pCurrent, currSch);
  if (!p) {
    p = make_product_join(pCurrent, currSch);
  }
  return p;
}

std::shared_ptr<plan>
table_planner::make_product_plan(const std::shared_ptr<plan> &pCurrent) {
  std::shared_ptr<plan> p = add_select_pred(mMyPlan);
  return std::static_pointer_cast<plan>(
      std::make_shared<multibuffer_product_plan>(mTx, pCurrent, p));
}

std::shared_ptr<plan> table_planner::make_index_select() {
  for (const auto &[fldName, iI] : mIndexes) {
    constant val = mMyPred.equates_with_constant(fldName);
    if (!val.is_null()) {
      std::cout << "index on " + fldName + " used" << std::endl;
      return std::static_pointer_cast<plan>(
          std::make_shared<index_select_plan>(mMyPlan, iI, val));
    }
  }
  return nullptr;
}

std::shared_ptr<plan>
table_planner::make_index_join(const std::shared_ptr<plan> &pCurrent,
                               const schema &pCurrSch) {
  for (const auto &[fldName, iI] : mIndexes) {
    std::string outerField = mMyPred.equates_with_field(fldName);
    if (!outerField.empty() && pCurrSch.has_field(outerField)) {
      auto p = std::static_pointer_cast<plan>(
          std::make_shared<index_join_plan>(pCurrent, mMyPlan, iI, outerField));
      p = add_select_pred(p);
      return add_join_pred(p, pCurrSch);
    }
  }
  return nullptr;
}

std::shared_ptr<plan>
table_planner::make_product_join(const std::shared_ptr<plan> &pCurrent,
                                 const schema &pCurrSch) {
  std::shared_ptr<plan> p = make_product_plan(pCurrent);
  return add_join_pred(p, pCurrSch);
}

std::shared_ptr<plan>
table_planner::add_select_pred(const std::shared_ptr<plan> &pP) {
  predicate selectPred = mMyPred.select_sub_pred(mMySchema);
  if (!selectPred.is_null()) {
    return std::static_pointer_cast<plan>(
        std::make_shared<select_plan>(pP, selectPred));
  } else {
    return pP;
  }
}

std::shared_ptr<plan>
table_planner::add_join_pred(const std::shared_ptr<plan> &pP,
                             const schema &pCurrSch) {
  predicate joinPred = mMyPred.join_sub_pred(pCurrSch, mMySchema);
  if (!joinPred.is_null()) {
    return std::static_pointer_cast<plan>(
        std::make_shared<select_plan>(pP, joinPred));
  } else {
    return pP;
  }
}
} // namespace simpledb

#include "opt/heuristicqueryplanner.hpp"
#include "plan/projectplan.hpp"
#include "utils/logger.hpp"

namespace simpledb {
heuristic_query_planner::heuristic_query_planner(metadata_manager &pMM)
    : mMM(pMM) {}

std::shared_ptr<plan> heuristic_query_planner::create_plan(query_data *pData,
                                                           transaction *pTx) {

  // Step 1: Create a TablePlanner object for each mentioned table
  for (const std::string &tblName : pData->tables()) {
    auto tP =
        std::make_shared<table_planner>(tblName, pData->pred(), *pTx, mMM);
    mTablePlanners.insert(tP);
  }

  // Step 2: Choose the lowest-size plan to begin the join order
  std::shared_ptr<plan> currentPlan = get_lowest_select_plan();

  // Step 3: Repeatedly add a plan to the join order
  while (!mTablePlanners.empty()) {
    std::shared_ptr<plan> p = get_lowest_join_plan(currentPlan);
    if (p) {
      currentPlan = p;
    } else {
      currentPlan = get_lowest_product_plan(currentPlan);
    }
  }

  // Step 4: Project on the field naems and rethrn
  return std::static_pointer_cast<plan>(
      std::make_shared<project_plan>(currentPlan, pData->fields()));
}

std::shared_ptr<plan> heuristic_query_planner::get_lowest_select_plan() {
  std::shared_ptr<table_planner> bestTP;
  std::shared_ptr<plan> bestPlan;
  for (const auto &tP : mTablePlanners) {
    std::shared_ptr<plan> p = tP->make_select_plan();
    if (!bestPlan || p->records_output() < bestPlan->records_output()) {
      bestTP = tP;
      bestPlan = p;
    }
  }
  mTablePlanners.erase(bestTP);
  return bestPlan;
}

std::shared_ptr<plan> heuristic_query_planner::get_lowest_join_plan(
    const std::shared_ptr<plan> &pCurrent) {
  std::shared_ptr<table_planner> bestTP;
  std::shared_ptr<plan> bestPlan;
  for (const auto &tP : mTablePlanners) {
    std::shared_ptr<plan> p = tP->make_join_plan(pCurrent);
    if (p && (!bestPlan || p->records_output() < bestPlan->records_output())) {
      bestTP = tP;
      bestPlan = p;
    }
  }
  if (bestPlan) {
    mTablePlanners.erase(bestTP);
  }
  return bestPlan;
}

std::shared_ptr<plan> heuristic_query_planner::get_lowest_product_plan(
    const std::shared_ptr<plan> &pCurrent) {
  std::shared_ptr<table_planner> bestTP;
  std::shared_ptr<plan> bestPlan;
  for (const auto &tP : mTablePlanners) {
    std::shared_ptr<plan> p = tP->make_product_plan(pCurrent);
    if (!bestPlan || p->records_output() < bestPlan->records_output()) {
      bestTP = tP;
      bestPlan = p;
    }
  }
  mTablePlanners.erase(bestTP);
  return bestPlan;
}

void heuristic_query_planner::set_planner(planner *pP) {}
} // namespace simpledb

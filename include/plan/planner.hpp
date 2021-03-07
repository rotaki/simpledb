#pragma once

#include <memory>

#include "plan/queryplanner.hpp"
#include "plan/updateplanner.hpp"

namespace smartdb {
  class planner {
  public:
    planner(std::unique_ptr<query_planner> pQP, std::unique_ptr<update_planner> pUP);
    std::shared_ptr<plan> create_query_plan(const std::string &pCMD, transaction* pTx);
    int execute_update(const std::string &pCMD, transaction* pTx);
  private:
    std::unique_ptr<query_planner> mQP;
    std::unique_ptr<update_planner> mUP;
  };
}


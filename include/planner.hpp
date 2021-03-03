#pragma once

#include <memory>

#include "queryplanner.hpp"
#include "updateplanner.hpp"

namespace smartdb {
  class planner {
  public:
    planner(std::shared_ptr<query_planner> pQP, std::shared_ptr<update_planner> pUP);
    std::shared_ptr<plan> create_query_plan(const std::string &pCMD, std::shared_ptr<transaction> pTx);
    int execute_update(const std::string &pCMD, std::shared_ptr<transaction> pTx);
  private:
    std::shared_ptr<query_planner> mQP;
    std::shared_ptr<update_planner> mUP;
  };
}


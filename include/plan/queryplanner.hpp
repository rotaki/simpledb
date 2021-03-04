#pragma once

#include "tx/transaction.hpp"
#include "parse/querydata.hpp"

namespace smartdb {
  class query_planner {
  public:
    virtual ~query_planner() {}
    virtual std::shared_ptr<plan> create_plan(std::shared_ptr<query_data> pQD, std::shared_ptr<transaction> pTx) = 0;
  };
} 

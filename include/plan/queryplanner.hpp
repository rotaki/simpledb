#pragma once

#include "parse/querydata.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class query_planner {
public:
  virtual ~query_planner() {}
  virtual std::shared_ptr<plan> create_plan(query_data *pQD,
                                            transaction *pTx) = 0;
};
} // namespace simpledb

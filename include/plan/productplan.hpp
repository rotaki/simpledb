#pragma once

#include <memory>

#include "plan/plan.hpp"

namespace simpledb {
class product_plan : public plan {
public:
  product_plan(const std::shared_ptr<plan> &pP1,
               const std::shared_ptr<plan> &pP2);
  std::shared_ptr<scan> open() override;
  int blocks_accessed() override;
  int records_output() override;
  int distinct_values(const std::string &pFldName) override;
  schema get_schema() override;

private:
  std::shared_ptr<plan> mP1, mP2;
  schema mSch;
};
} // namespace simpledb

#pragma once

#include "metadata/indexmanager.hpp"
#include "plan/plan.hpp"

namespace simpledb {
class index_select_plan : public plan {
public:
  index_select_plan(const std::shared_ptr<plan> &pP, const index_info &pII,
                    const constant &pVal);
  std::shared_ptr<scan> open() override;
  int blocks_accessed() override;
  int records_output() override;
  int distinct_values(const std::string &pFldName) override;
  schema get_schema() override;

private:
  std::shared_ptr<plan> mP;
  index_info mII;
  constant mVal;
};
} // namespace simpledb

#pragma once

#include <memory>

#include "plan/plan.hpp"
#include "metadata/indexmanager.hpp"

namespace simpledb {
  class index_join_plan : public plan{
  public:
    index_join_plan(const std::shared_ptr<plan> &pP1, const std::shared_ptr<plan> &pP2, const index_info &pII, const std::string &pJoinField);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    schema get_schema() override;
  private:
    std::shared_ptr<plan> mP1, mP2;
    index_info mII;
    std::string mJoinField;
    schema mSch;
  };
}

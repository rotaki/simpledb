#pragma once

#include <memory>

#include "plan.hpp"
#include "indexmanager.hpp"

namespace smartdb {
  class index_join_plan : public plan{
  public:
    index_join_plan(std::shared_ptr<plan> pP1, std::shared_ptr<plan> pP2, std::shared_ptr<index_info> pII, const std::string &pJoinField);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    std::shared_ptr<schema> get_schema() override;
  private:
    std::shared_ptr<plan> mP1, mP2;
    std::shared_ptr<index_info> mII;
    std::string mJoinField;
    std::shared_ptr<schema> mSch;
  };
}

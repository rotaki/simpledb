#pragma once

#include <memory>
#include <vector>
#include <string>

#include "plan/plan.hpp"

namespace smartdb {
  class project_plan: public plan {
  public:
    project_plan(std::shared_ptr<plan> pP, std::vector<std::string> pFieldList);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    std::shared_ptr<schema> get_schema() override;
  private:
    std::shared_ptr<plan> mP;
    std::shared_ptr<schema> mSch;
  };
}

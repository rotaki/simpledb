#pragma once

#include <memory>

#include "plan.hpp"


namespace smartdb {
  class product_plan:public plan {
  public:
    product_plan(std::shared_ptr<plan> pP1, std::shared_ptr<plan> pP2);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    std::shared_ptr<schema> get_schema() override;
  private:
    std::shared_ptr<plan> mP1, mP2;
    std::shared_ptr<schema> mSch;
  };
}

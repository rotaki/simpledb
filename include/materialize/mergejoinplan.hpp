#pragma once

#include <string>

#include "tx/transaction.hpp"
#include "plan/plan.hpp"
#include "query/scan.hpp"
#include "record/schema.hpp"

namespace simpledb {
  class merge_join_plan: public plan {
  public:
    merge_join_plan(transaction* pTx, const std::shared_ptr<plan> &pP1,
                    const std::shared_ptr<plan> &pP2, const std::string &pFldName1,
                    const std::string &pFldName2);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    schema get_schema() override;
  private:
    std::shared_ptr<plan> mP1, mP2;
    std::string mFldName1, mFldName2;
    schema mSch;
  };
}

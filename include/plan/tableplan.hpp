#pragma once

#include <memory>

#include "tx/transaction.hpp"
#include "plan/plan.hpp"
#include "metadata/metadatamanager.hpp"
#include "query/predicate.hpp"

namespace smartdb {
  class table_plan: public plan {
  public:
    table_plan(transaction* pTx, const std::string &pTblName, metadata_manager* pMM);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    schema get_schema() override;
  private:
    transaction* mTx;
    std::string mTblName;
    layout mLt;
    stat_info mSI;
  };
}


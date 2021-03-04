#pragma once

#include <memory>

#include "tx/transaction.hpp"
#include "plan/plan.hpp"
#include "metadata/metadatamanager.hpp"
#include "query/predicate.hpp"

namespace smartdb {
  class table_plan: public plan {
  public:
    table_plan(std::shared_ptr<transaction> pTx, const std::string &pTblName, std::shared_ptr<metadata_manager> pMM);
    std::shared_ptr<scan> open() override;
    int blocks_accessed() override;
    int records_output() override;
    int distinct_values(const std::string &pFldName) override;
    std::shared_ptr<schema> get_schema() override;
  private:
    std::shared_ptr<transaction> mTx;
    std::string mTblName;
    std::shared_ptr<layout> mLt;
    std::shared_ptr<stat_info> mSI;
  };
}


#pragma once

#include "tx/transaction.hpp"
#include "plan/queryplanner.hpp"
#include "metadata/metadatamanager.hpp"

namespace smartdb {
  class basic_query_planner: public query_planner {
  public:
    basic_query_planner(metadata_manager* pMM);
    std::shared_ptr<plan> create_plan(query_data* pData, transaction* pTx) override;
  private:
    metadata_manager* mMM;
  };
}

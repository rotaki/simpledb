#pragma once

#include "tx/transaction.hpp"
#include "plan/queryplanner.hpp"
#include "metadata/metadatamanager.hpp"

namespace smartdb {
  class basic_query_planner: public query_planner {
  public:
    basic_query_planner(std::shared_ptr<metadata_manager> pMM);
    std::shared_ptr<plan> create_plan(std::shared_ptr<query_data> pData, std::shared_ptr<transaction> pTx) override;
  private:
    std::shared_ptr<metadata_manager> mMM;
  };
}

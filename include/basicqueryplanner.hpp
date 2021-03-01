#pragma once

#include "transaction.hpp"
#include "queryplanner.hpp"
#include "metadatamanager.hpp"

namespace smartdb {
  class basic_query_planner: public query_planner {
  public:
    basic_query_planner(std::shared_ptr<metadata_manager> pMM);
    std::shared_ptr<plan> create_plan(std::shared_ptr<query_data> pData, std::shared_ptr<transaction> pTx) override;
  private:
    std::shared_ptr<metadata_manager> mMM;
  };
}

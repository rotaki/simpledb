#pragma once

#include "metadata/metadatamanager.hpp"
#include "plan/queryplanner.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class basic_query_planner : public query_planner {
public:
  basic_query_planner(metadata_manager *pMM);
  std::shared_ptr<plan> create_plan(query_data *pData,
                                    transaction *pTx) override;

private:
  metadata_manager *mMM;
};
} // namespace simpledb

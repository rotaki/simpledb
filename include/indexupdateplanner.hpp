#pragma once

#include "updateplanner.hpp"
#include "metadatamanager.hpp"

namespace smartdb {
  class index_update_planner: public update_planner {
  public:
    index_update_planner(std::shared_ptr<metadata_manager> pMM);
    int execute_insert(std::shared_ptr<insert_data> pID, std::shared_ptr<transaction> pTx) override;
    int execute_delete(std::shared_ptr<delete_data> pDD, std::shared_ptr<transaction> pTx) override;
    int execute_modify(std::shared_ptr<modify_data> pMD, std::shared_ptr<transaction> pTx) override;
    int execute_create_table(std::shared_ptr<create_table_data> pCTD, std::shared_ptr<transaction> pTx) override;
    int execute_create_view(std::shared_ptr<create_view_data> pCVD, std::shared_ptr<transaction> pTx) override;
    int execute_create_index(std::shared_ptr<create_index_data> pCID, std::shared_ptr<transaction> pTx) override;
  private:
    std::shared_ptr<metadata_manager> mMM;
  };
}
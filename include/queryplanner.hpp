#pragma once

#include "transaction.hpp"
#include "querydata.hpp"
#include "insertdata.hpp"
#include "deletedata.hpp"
#include "modifydata.hpp"
#include "createtabledata.hpp"
#include "createviewdata.hpp"
#include "createindexdata.hpp"

namespace smartdb {
  class query_planner {
  public:
    virtual ~query_planner() {}
    virtual std::shared_ptr<plan> create_plan(std::shared_ptr<query_data> pQD, std::shared_ptr<transaction> pTx) = 0;
  };

  class update_planner {
  public:
    virtual ~update_planner() {}
    virtual int execute_insert(std::shared_ptr<insert_data> pID, std::shared_ptr<transaction> pTx) = 0;
    virtual int execute_delete(std::shared_ptr<delete_data> pDD, std::shared_ptr<transaction> pTx) = 0;
    virtual int execute_modify(std::shared_ptr<modify_data> pMD, std::shared_ptr<transaction> pTx) = 0;
    virtual int execute_create_table(std::shared_ptr<create_table_data> pCTD, std::shared_ptr<transaction> pTx) = 0;
    virtual int execute_create_view(std::shared_ptr<create_view_data> pCVD, std::shared_ptr<transaction> pTx) = 0;
    virtual int execute_create_index(std::shared_ptr<create_index_data> pCID, std::shared_ptr<transaction> pTx) = 0;
  };
} 

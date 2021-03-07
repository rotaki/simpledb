#pragma once

#include "tx/transaction.hpp"
#include "parse/insertdata.hpp"
#include "parse/deletedata.hpp"
#include "parse/modifydata.hpp"
#include "parse/createtabledata.hpp"
#include "parse/createviewdata.hpp"
#include "parse/createindexdata.hpp"

namespace smartdb {
class update_planner {
  public:
    virtual ~update_planner() {}
    virtual int execute_insert(insert_data* pID, transaction* pTx) = 0;
    virtual int execute_delete(delete_data* pDD, transaction* pTx) = 0;
    virtual int execute_modify(modify_data* pMD, transaction* pTx) = 0;
    virtual int execute_create_table(create_table_data* pCTD, transaction* pTx) = 0;
    virtual int execute_create_view(create_view_data* pCVD, transaction* pTx) = 0;
    virtual int execute_create_index(create_index_data* pCID, transaction* pTx) = 0;
  };
}

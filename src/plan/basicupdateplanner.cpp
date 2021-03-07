#include <iostream>

#include "plan/basicupdateplanner.hpp"
#include "plan/tableplan.hpp"
#include "plan/selectplan.hpp"

namespace simpledb {
  basic_update_planner::basic_update_planner(metadata_manager* pMM):
    mMM(pMM) {}
  
  int basic_update_planner::execute_delete(delete_data* pData, transaction* pTx) {
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::make_shared<table_plan>(pTx, pData->table_name(), mMM));
    p = std::static_pointer_cast<plan>(std::make_shared<select_plan>(p, pData->pred()));
    std::shared_ptr<update_scan> us = std::static_pointer_cast<update_scan>(p->open());
    int count = 0;
    while (us->next()) {
      us->remove();
      count++;
    }
    us->close();
    return count;
  }
  
  int basic_update_planner::execute_modify(modify_data* pData, transaction* pTx) {
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::make_shared<table_plan>(pTx, pData->table_name(), mMM));
    p = std::static_pointer_cast<plan>(std::make_shared<select_plan>(p, pData->pred()));
    std::shared_ptr<update_scan> us = std::static_pointer_cast<update_scan>(p->open());
    int count = 0;
    while (us->next()) {
      constant val = pData->new_value().evaluate(us.get());
      us->set_val(pData->target_field(), val);
      count++;
    }
    us->close();
    return count;
  }
  
  int basic_update_planner::execute_insert(insert_data* pData, transaction* pTx) {
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::make_shared<table_plan>(pTx, pData->table_name(), mMM));
    std::shared_ptr<update_scan> us = std::static_pointer_cast<update_scan>(p->open());
    us->insert();
    std::vector<constant> v = pData->vals();
    std::vector<std::string> f = pData->fields();
    for (int i = 0; i < static_cast<int>(v.size()); i++) {
      us->set_val(f[i], v[i]);
    }
    us->close();
    return 1;
  }

  int basic_update_planner::execute_create_table(create_table_data* pData, transaction* pTx) {
    mMM->create_table(pData->table_name(), pData->new_schema(), pTx);
    return 0;
  }
  
  int basic_update_planner::execute_create_view(create_view_data* pData, transaction* pTx) {
    mMM->create_view(pData->view_name(), pData->view_def(), pTx);
    return 0;
  }
  
  int basic_update_planner::execute_create_index(create_index_data* pData, transaction* pTx) {
    mMM->create_index(pData->index_name(), pData->table_name(), pData->field_name(), pTx);
    return 0;
  }
}

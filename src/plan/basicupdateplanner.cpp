#include <iostream>

#include "plan/basicupdateplanner.hpp"
#include "plan/tableplan.hpp"
#include "plan/selectplan.hpp"

namespace smartdb {
  basic_update_planner::basic_update_planner(std::shared_ptr<metadata_manager> pMM):
    mMM(pMM) {}
  
  int basic_update_planner::execute_delete(std::shared_ptr<delete_data> pData, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, pData->table_name(), mMM)));
    p = std::static_pointer_cast<plan>(std::shared_ptr<select_plan>(new select_plan(p, pData->pred())));
    std::shared_ptr<update_scan> us = std::static_pointer_cast<update_scan>(p->open()); // ok?
    int count = 0;
    while (us->next()) {
      us->remove();
      count++;
    }
    us->close();
    return count;
  }
  
  int basic_update_planner::execute_modify(std::shared_ptr<modify_data> pData, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, pData->table_name(), mMM)));
    p = std::static_pointer_cast<plan>(std::shared_ptr<select_plan>(new select_plan(p, pData->pred())));
    std::shared_ptr<update_scan> us = std::static_pointer_cast<update_scan>(p->open()); // ok?
    int count = 0;
    while (us->next()) {
      constant val = pData->new_value().evaluate(us);
      us->set_val(pData->target_field(), val);
      count++;
    }
    us->close();
    return count;
  }
  
  int basic_update_planner::execute_insert(std::shared_ptr<insert_data> pData, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<plan> p = std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, pData->table_name(), mMM)));
    std::shared_ptr<update_scan> us = std::static_pointer_cast<update_scan>(p->open()); // ok?
    us->insert();
    std::vector<constant> v = pData->vals();
    std::vector<std::string> f = pData->fields();
    for (int i = 0; i < static_cast<int>(v.size()); i++) {
      us->set_val(f[i], v[i]);
    }
    us->close();
    return 1;
  }

  int basic_update_planner::execute_create_table(std::shared_ptr<create_table_data> pData, std::shared_ptr<transaction> pTx) {
    mMM->create_table(pData->table_name(), pData->new_schema(), pTx);
    return 0;
  }
  
  int basic_update_planner::execute_create_view(std::shared_ptr<create_view_data> pData, std::shared_ptr<transaction> pTx) {
    mMM->create_view(pData->view_name(), pData->view_def(), pTx);
    return 0;
  }
  
  int basic_update_planner::execute_create_index(std::shared_ptr<create_index_data> pData, std::shared_ptr<transaction> pTx) {
    mMM->create_index(pData->index_name(), pData->table_name(), pData->field_name(), pTx);
    return 0;
  }
}

#include <iostream>
#include "planner.hpp"
#include "object.hpp"
#include "parser.hpp"

namespace smartdb {
  planner::planner(std::shared_ptr<query_planner> pQP, std::shared_ptr<update_planner> pUP):
    mQP(pQP), mUP(pUP) {}

  std::shared_ptr<plan> planner::create_query_plan(const std::string &pCMD, std::shared_ptr<transaction> pTx) {
    parser p(pCMD);
    std::shared_ptr<query_data> data = p.query();
    return mQP->create_plan(data, pTx);
  }

  int planner::execute_update(const std::string &pCMD, std::shared_ptr<transaction> pTx) {
    parser p(pCMD);
    std::shared_ptr<object> obj = p.update_cmd();
    if (obj->op() == object::insert) {
      return mUP->execute_insert(std::static_pointer_cast<insert_data>(obj), pTx);
    } else if (obj->op() == object::remove) {
      return mUP->execute_delete(std::static_pointer_cast<delete_data>(obj), pTx);
    } else if (obj->op() == object::modify) {
      return mUP->execute_modify(std::static_pointer_cast<modify_data>(obj), pTx);
    } else if (obj->op() == object::createtable) {
      return mUP->execute_create_table(std::static_pointer_cast<create_table_data>(obj), pTx);
    } else if (obj->op() == object::createview) {
      return mUP->execute_create_view(std::static_pointer_cast<create_view_data>(obj), pTx);
    } else if (obj->op() == object::createindex) {
      return mUP->execute_create_index(std::static_pointer_cast<create_index_data>(obj), pTx);
    } else {
      return 0;
    }
  }
}

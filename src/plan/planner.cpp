#include <iostream>

#include "plan/planner.hpp"
#include "parse/object.hpp"
#include "parse/parser.hpp"

namespace smartdb {
  planner::planner(std::unique_ptr<query_planner> pQP, std::unique_ptr<update_planner> pUP):
    mQP(std::move(pQP)), mUP(std::move(pUP)) {}

  std::shared_ptr<plan> planner::create_query_plan(const std::string &pCMD, transaction* pTx) {
    parser p(pCMD);
    std::unique_ptr<query_data> data = p.query();
    return mQP->create_plan(data.get(), pTx);
  }

  int planner::execute_update(const std::string &pCMD, transaction* pTx) {
    parser p(pCMD);
    std::unique_ptr<object> obj = p.update_cmd();
    if (obj->op() == object::insert) {
      return mUP->execute_insert(static_cast<insert_data*>(obj.get()), pTx);
    } else if (obj->op() == object::remove) {
      return mUP->execute_delete(static_cast<delete_data*>(obj.get()), pTx);
    } else if (obj->op() == object::modify) {
      return mUP->execute_modify(static_cast<modify_data*>(obj.get()), pTx);
    } else if (obj->op() == object::createtable) {
      return mUP->execute_create_table(static_cast<create_table_data*>(obj.get()), pTx);
    } else if (obj->op() == object::createview) {
      return mUP->execute_create_view(static_cast<create_view_data*>(obj.get()), pTx);
    } else if (obj->op() == object::createindex) {
      return mUP->execute_create_index(static_cast<create_index_data*>(obj.get()), pTx);
    } else {
      return 0;
    }
  }
}

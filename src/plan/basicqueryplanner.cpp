#include "plan/basicqueryplanner.hpp"
#include "plan/tableplan.hpp"
#include "plan/productplan.hpp"
#include "plan/selectplan.hpp"
#include "plan/projectplan.hpp"
#include "parse/parser.hpp"

namespace smartdb {
  basic_query_planner::basic_query_planner(std::shared_ptr<metadata_manager> pMM):
    mMM(pMM) {}

  std::shared_ptr<plan> basic_query_planner::create_plan(std::shared_ptr<query_data> pData, std::shared_ptr<transaction> pTx) {
    // step1 create a plan for each menthioned table or view
    std::vector<std::shared_ptr<plan>> plans;
    for (const std::string &tblName: pData->tables()) {
      std::string viewDef = mMM->get_view_def(tblName, pTx);
      if (!viewDef.empty()) {
        parser p(viewDef);
        std::shared_ptr<query_data> viewData = p.query();
        plans.emplace_back(create_plan(viewData, pTx));
      } else {
        plans.emplace_back(std::static_pointer_cast<plan>(std::shared_ptr<table_plan>(new table_plan(pTx, tblName,  mMM))));
      }
    }
    // step2 create the product of all table plans
    std::shared_ptr<plan> p = plans[0];
    for (int i = 1; i < static_cast<int>(plans.size()); i++) {
      p = std::static_pointer_cast<plan>(std::shared_ptr<product_plan>(new product_plan(p, plans[i])));
    }

    // step3 add a selection plan for the predicate
    p = std::static_pointer_cast<plan>(std::shared_ptr<select_plan>(new select_plan(p, pData->pred())));

    // step4 project on the field names
    return std::static_pointer_cast<plan>(std::shared_ptr<project_plan>(new project_plan(p, pData->fields())));
  }
}

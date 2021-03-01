#include "selectplan.hpp"
#include "selectscan.hpp"

namespace smartdb {
  select_plan::select_plan(std::shared_ptr<plan> pPlan, const predicate &pPred):
    mPlan(pPlan), mPred(pPred) {}

  std::shared_ptr<scan> select_plan::open() {
    std::shared_ptr<scan> s = mPlan->open();
    return std::static_pointer_cast<scan>(std::shared_ptr<select_scan>(new select_scan(s, mPred)));
  }

  int select_plan::blocks_accessed() {
    return mPlan->blocks_accessed();
  }

  int select_plan::records_output() {
    return mPlan->records_output() / mPred.reduction_factor(mPlan);
  }

  int select_plan::distinct_values(const std::string &pFldName) {
    if (!mPred.equates_with_constant(pFldName).is_null()) {
      return 1;
    } else {
      std::string fldName = mPred.equates_with_field(pFldName);
      if (!fldName.empty()) {
        return std::min(mPlan->distinct_values(pFldName), mPlan->distinct_values(fldName));
      } else {
        return mPlan->distinct_values(pFldName);
      }
    }
  }

  std::shared_ptr<schema> select_plan::get_schema() {
    return mPlan->get_schema();
  }
}

#include "plan/selectplan.hpp"
#include "query/selectscan.hpp"

namespace smartdb {
  select_plan::select_plan(const std::shared_ptr<plan> &pPlan, const predicate &pPred):
    mPlan(pPlan), mPred(pPred) {}

  std::shared_ptr<scan> select_plan::open() {
    std::shared_ptr<scan> s = mPlan->open();
    return std::static_pointer_cast<scan>(std::make_shared<select_scan>(s, mPred));
  }

  int select_plan::blocks_accessed() {
    return mPlan->blocks_accessed();
  }

  int select_plan::records_output() {
    return mPlan->records_output() / mPred.reduction_factor(mPlan.get());
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

  schema select_plan::get_schema() {
    return mPlan->get_schema();
  }
}

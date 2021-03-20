#include "materialize/groupbyplan.hpp"
#include "materialize/groupbyscan.hpp"
#include "materialize/sortplan.hpp"

namespace simpledb {
group_by_plan::group_by_plan(
    transaction *pTx, const std::shared_ptr<plan> &pP,
    const std::vector<std::string> &pGroupFields,
    const std::vector<std::shared_ptr<aggregation_fn>> &pAggFns)
    : mGroupFields(pGroupFields), mAggFns(pAggFns) {
  mP = std::static_pointer_cast<plan>(
      std::make_shared<sort_plan>(pTx, pP, mGroupFields));
  for (const std::string &fldName : mGroupFields) {
    mSch.add(fldName, mP->get_schema());
  }
  for (const auto &fn : mAggFns) {
    mSch.add_int_field(fn->field_name());
  }
}

std::shared_ptr<scan> group_by_plan::open() {
  std::shared_ptr<scan> s = mP->open();
  return std::static_pointer_cast<scan>(
      std::make_shared<group_by_scan>(s, mGroupFields, mAggFns));
}

int group_by_plan::blocks_accessed() { return mP->blocks_accessed(); }

int group_by_plan::records_output() {
  int numGroups = 1;
  for (const std::string &fldName : mGroupFields) {
    numGroups *= mP->distinct_values(fldName);
  }
  return numGroups;
}

int group_by_plan::distinct_values(const std::string &pFldName) {
  if (mP->get_schema().has_field(pFldName)) {
    return mP->distinct_values(pFldName);
  } else {
    return records_output();
  }
}

schema group_by_plan::get_schema() { return mSch; }

} // namespace simpledb

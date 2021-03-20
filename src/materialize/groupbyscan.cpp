#include <algorithm>

#include "materialize/groupbyscan.hpp"

namespace simpledb {
group_by_scan::group_by_scan(
    const std::shared_ptr<scan> &pS,
    const std::vector<std::string> &pGroupFields,
    const std::vector<std::shared_ptr<aggregation_fn>> &pAggFns)
    : mS(pS), mGroupFields(pGroupFields), mAggFns(pAggFns) {
  mS->before_first();
  mMoreGroups = mS->next();
}

void group_by_scan::before_first() {
  mS->before_first();
  mMoreGroups = mS->next();
}

bool group_by_scan::next() {
  if (!mMoreGroups) {
    return false;
  }
  for (const auto &fn : mAggFns) {
    fn->process_first(mS.get());
  }
  mGroupVal = std::make_unique<group_value>(mS.get(), mGroupFields);
  while ((mMoreGroups = mS->next())) {
    auto gv = std::make_unique<group_value>(mS.get(), mGroupFields);
    if (*mGroupVal != *gv) {
      break;
    }
    for (const auto &fn : mAggFns) {
      fn->process_next(mS.get());
    }
  }
  return true;
}

void group_by_scan::close() { mS->close(); }

constant group_by_scan::get_val(const std::string &pFldName) {
  if (std::find(mGroupFields.begin(), mGroupFields.end(), pFldName) !=
      mGroupFields.end()) {
    return mGroupVal->get_val(pFldName);
  }
  for (const auto &fn : mAggFns) {
    if (fn->field_name() == pFldName) {
      return fn->value();
    }
  }
  throw std::runtime_error("field " + pFldName + " not found.");
}

int group_by_scan::get_int(const std::string &pFldName) {
  return get_val(pFldName).as_int();
}

std::string group_by_scan::get_string(const std::string &pFldName) {
  return get_val(pFldName).as_string();
}

bool group_by_scan::has_field(const std::string &pFldName) {
  if (std::find(mGroupFields.begin(), mGroupFields.end(), pFldName) !=
      mGroupFields.end()) {
    return true;
  }
  for (const auto &fn : mAggFns) {
    if (fn->field_name() == pFldName) {
      return true;
    }
  }
  return false;
}
} // namespace simpledb

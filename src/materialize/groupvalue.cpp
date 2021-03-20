#include "materialize/groupvalue.hpp"

namespace simpledb {
bool operator==(const group_value &pR, const group_value &pL) {
  for (const auto &[fldName, valueR] : pR.mVals) {
    if (pL.mVals.find(fldName) == pL.mVals.end() ||
        pL.mVals.at(fldName) != valueR) {
      return false;
    }
  }
  return true;
}

bool operator!=(const group_value &pR, const group_value &pL) {
  if (pR == pL) {
    return false;
  } else {
    return true;
  }
}

group_value::group_value(scan *s, const std::vector<std::string> &pFields) {
  for (const std::string &fldName : pFields) {
    mVals[fldName] = s->get_val(fldName);
  }
}

constant group_value::get_val(const std::string &pFldName) {
  return mVals.at(pFldName);
}

int group_value::hash_code() {
  int hashVal = 0;
  for (const auto &[fldName, value] : mVals) {
    hashVal += value.hash_code();
  }
  return hashVal;
}

} // namespace simpledb

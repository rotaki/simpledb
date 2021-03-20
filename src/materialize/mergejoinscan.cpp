#include "materialize/mergejoinscan.hpp"

namespace simpledb {
merge_join_scan::merge_join_scan(const std::shared_ptr<scan> &pS1,
                                 const std::shared_ptr<sort_scan> &pS2,
                                 const std::string &pFldName1,
                                 const std::string &pFldName2)
    : mS1(pS1), mS2(pS2), mFldName1(pFldName1), mFldName2(pFldName2) {
  mS1->before_first();
  mS2->before_first();
}

void merge_join_scan::close() {
  mS1->close();
  mS2->close();
}

void merge_join_scan::before_first() {
  mS1->before_first();
  mS2->before_first();
}

bool merge_join_scan::next() {
  bool hasMore2 = mS2->next();
  if (hasMore2 && mS2->get_val(mFldName2) == mJoinVal) {
    return true;
  }
  bool hasMore1 = mS1->next();
  if (hasMore1 && mS1->get_val(mFldName1) == mJoinVal) {
    mS2->restore_position();
    return true;
  }

  while (hasMore1 && hasMore2) {
    constant v1 = mS1->get_val(mFldName1);
    constant v2 = mS2->get_val(mFldName2);
    if (v1 < v2) {
      hasMore1 = mS1->next();
    } else if (v2 < v1) {
      hasMore2 = mS2->next();
    } else {
      mS2->save_position();
      mJoinVal = mS2->get_val(mFldName2);
      return true;
    }
  }

  return false;
}

int merge_join_scan::get_int(const std::string &pFldName) {
  if (mS1->has_field(pFldName)) {
    return mS1->get_int(pFldName);
  } else {
    return mS2->get_int(pFldName);
  }
}

std::string merge_join_scan::get_string(const std::string &pFldName) {
  if (mS1->has_field(pFldName)) {
    return mS1->get_string(pFldName);
  } else {
    return mS2->get_string(pFldName);
  }
}

constant merge_join_scan::get_val(const std::string &pFldName) {
  if (mS1->has_field(pFldName)) {
    return mS1->get_val(pFldName);
  } else {
    return mS2->get_val(pFldName);
  }
}

bool merge_join_scan::has_field(const std::string &pFldName) {
  return mS1->has_field(pFldName) || mS2->has_field(pFldName);
}
} // namespace simpledb

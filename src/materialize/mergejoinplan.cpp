#include <vector>

#include "materialize/mergejoinplan.hpp"
#include "materialize/mergejoinscan.hpp"
#include "materialize/sortplan.hpp"

namespace simpledb {
merge_join_plan::merge_join_plan(transaction *pTx,
                                 const std::shared_ptr<plan> &pP1,
                                 const std::shared_ptr<plan> &pP2,
                                 const std::string &pFldName1,
                                 const std::string &pFldName2)
    : mFldName1(pFldName1), mFldName2(pFldName2) {
  std::vector<std::string> sortList1 = {mFldName1};
  mP1 = std::static_pointer_cast<plan>(
      std::make_shared<sort_plan>(pTx, pP1, sortList1));
  std::vector<std::string> sortList2 = {mFldName2};
  mP2 = std::static_pointer_cast<plan>(
      std::make_shared<sort_plan>(pTx, mP2, sortList2));
  mSch.add_all(mP1->get_schema());
  mSch.add_all(mP2->get_schema());
}

std::shared_ptr<scan> merge_join_plan::open() {
  std::shared_ptr<scan> s1 = mP1->open();
  auto s2 = std::static_pointer_cast<sort_scan>(mP2->open());
  return std::static_pointer_cast<scan>(
      std::make_shared<merge_join_scan>(s1, s2, mFldName1, mFldName2));
}

int merge_join_plan::blocks_accessed() {
  return mP1->blocks_accessed() + mP2->blocks_accessed();
}

int merge_join_plan::records_output() {
  int maxVals = std::max(mP1->distinct_values(mFldName1),
                         mP2->distinct_values(mFldName2));
  return (mP1->records_output() * mP2->records_output()) / maxVals;
}

int merge_join_plan::distinct_values(const std::string &pFldName) {
  if (mP1->get_schema().has_field(pFldName)) {
    return mP1->distinct_values(pFldName);
  } else {
    return mP2->distinct_values(pFldName);
  }
}

schema merge_join_plan::get_schema() { return mSch; }
} // namespace simpledb

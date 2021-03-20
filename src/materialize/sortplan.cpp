#include "materialize/sortplan.hpp"

namespace simpledb {
sort_plan::sort_plan(transaction *pTx, const std::shared_ptr<plan> &pP,
                     const std::vector<std::string> &pSortFields)
    : mP(pP), mTx(pTx), mSch(mP->get_schema()), mComp(pSortFields) {}

std::shared_ptr<scan> sort_plan::open() {
  std::shared_ptr<scan> src = mP->open();
  std::vector<std::shared_ptr<temp_table>> runs = split_into_runs(src.get());
  src->close();
  while (runs.size() > 2) {
    runs = do_a_merge_iteration(runs);
  }
  return std::static_pointer_cast<scan>(
      std::make_shared<sort_scan>(runs, mComp)); // todo
}

int sort_plan::blocks_accessed() {
  // does not include the one-time cost of sorting
  auto mp = std::static_pointer_cast<plan>(
      std::make_shared<materialize_plan>(mTx, mP));
  return mp->blocks_accessed();
}

int sort_plan::records_output() { return mP->records_output(); }

int sort_plan::distinct_values(const std::string &pFldName) {
  return mP->distinct_values(pFldName);
}

std::vector<std::shared_ptr<temp_table>>
sort_plan::split_into_runs(scan *pSrc) {
  std::vector<std::shared_ptr<temp_table>> temps;
  pSrc->before_first();
  if (!pSrc->next()) {
    return temps;
  }
  auto currentTemp = std::make_shared<temp_table>(mTx, mSch);
  temps.emplace_back(currentTemp);
  std::shared_ptr<update_scan> currentScan = currentTemp->open();
  while (copy(pSrc, currentScan.get())) {
    if (mComp.compare(pSrc, currentScan.get()) < 0) {
      // start new run
      currentScan->close();
      currentTemp = std::make_shared<temp_table>(mTx, mSch);
      temps.emplace_back(currentTemp);
      currentScan = std::static_pointer_cast<update_scan>(currentTemp->open());
    }
  }

  currentScan->close();
  return temps;
}

std::vector<std::shared_ptr<temp_table>> sort_plan::do_a_merge_iteration(
    const std::vector<std::shared_ptr<temp_table>> &pRuns) {
  std::vector<std::shared_ptr<temp_table>> result;
  for (int i = 0; i + 2 <= pRuns.size(); i += 2) {
    auto p1 = pRuns[i];
    auto p2 = pRuns[i + 1];
    result.emplace_back(merge_two_runs(p1.get(), p2.get()));
  }
  if (pRuns.size() % 2 == 1) {
    result.emplace_back(pRuns.back());
  }
  return result;
}

std::shared_ptr<temp_table> sort_plan::merge_two_runs(temp_table *pP1,
                                                      temp_table *pP2) {
  std::shared_ptr<scan> src1 = pP1->open();
  std::shared_ptr<scan> src2 = pP2->open();
  auto result = std::make_shared<temp_table>(mTx, mSch);
  std::shared_ptr<update_scan> dest = result->open();

  bool hasMore1 = src1->next();
  bool hasMore2 = src2->next();
  while (hasMore1 && hasMore2) {
    if (mComp.compare(src1.get(), src2.get()) < 0) {
      hasMore1 = copy(src1.get(), dest.get());
    } else {
      hasMore2 = copy(src2.get(), dest.get());
    }
  }

  if (hasMore1) {
    while (hasMore1) {
      hasMore1 = copy(src1.get(), dest.get());
    }
  } else {
    while (hasMore2) {
      hasMore2 = copy(src2.get(), dest.get());
    }
  }

  src1->close();
  src2->close();
  dest->close();
  return result;
}

bool sort_plan::copy(scan *pSrc, update_scan *pDest) {
  pDest->insert();
  for (const std::string &fldName : mSch.fields()) {
    pDest->set_val(fldName, pSrc->get_val(fldName));
  }
  return pSrc->next();
}
} // namespace simpledb

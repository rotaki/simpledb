#include "materialize/sortscan.hpp"

namespace simpledb {
sort_scan::sort_scan(const std::vector<std::shared_ptr<temp_table>> &pRuns,
                     record_comparator &pComp)
    : mComp(pComp), mHasMore2(false) {
  mS1 = std::static_pointer_cast<update_scan>(pRuns[0]->open());
  mHasMore1 = mS1->next();
  if (pRuns.size() > 1) {
    mS2 = std::static_pointer_cast<update_scan>(pRuns[1]->open());
    mHasMore2 = mS2->next();
  }
}

void sort_scan::before_first() {
  mS1->before_first();
  mHasMore1 = mS1->next();
  if (mS2) {
    mS2->before_first();
    mHasMore2 = mS2->next();
  }
}

bool sort_scan::next() {
  if (mCurrentScan == mS1) {
    mHasMore1 = mS1->next();
  } else if (mCurrentScan == mS2) {
    mHasMore2 = mS2->next();
  }

  if (!mHasMore1 && !mHasMore2) {
    return false;
  } else if (mHasMore1 && mHasMore2) {
    if (mComp.compare(mS1.get(), mS2.get()) < 0) {
      mCurrentScan = mS1;
    } else {
      mCurrentScan = mS2;
    }
  } else if (mHasMore1) {
    mCurrentScan = mS1;
  } else if (mHasMore2) {
    mCurrentScan = mS2;
  }
  return true;
}

void sort_scan::close() {
  mS1->close();
  if (mS2) {
    mS2->close();
  }
}

constant sort_scan::get_val(const std::string &pFldName) {
  return mCurrentScan->get_val(pFldName);
}

int sort_scan::get_int(const std::string &pFldName) {
  return mCurrentScan->get_int(pFldName);
}

std::string sort_scan::get_string(const std::string &pFldName) {
  return mCurrentScan->get_string(pFldName);
}

bool sort_scan::has_field(const std::string &pFldName) {
  return mCurrentScan->has_field(pFldName);
}

void sort_scan::save_position() {
  rid r1 = mS1->get_rid();
  rid r2 = mS2->get_rid();
  mSavedPosition = std::vector<rid>{r1, r2};
}

void sort_scan::restore_position() {
  rid r1 = mSavedPosition[0];
  rid r2 = mSavedPosition[1];
  mS1->move_to_rid(r1);
  mS2->move_to_rid(r2);
}

} // namespace simpledb

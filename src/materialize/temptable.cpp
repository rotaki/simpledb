#include "materialize/temptable.hpp"

namespace simpledb {
int temp_table::mNextTableNum = 0;
std::mutex temp_table::mMutex;

temp_table::temp_table(transaction *pTx, const schema &pSch)
    : mTx(pTx), mTblName(next_table_name()), mLayout(pSch) {}

std::shared_ptr<update_scan> temp_table::open() {
  return std::static_pointer_cast<update_scan>(
      std::make_shared<table_scan>(mTx, mTblName, mLayout));
}

std::string temp_table::table_name() { return mTblName; }

layout temp_table::get_layout() { return mLayout; }

std::string temp_table::next_table_name() {
  std::unique_lock lock(mMutex);
  mNextTableNum++;
  return "temp" + std::to_string(mNextTableNum);
}
} // namespace simpledb

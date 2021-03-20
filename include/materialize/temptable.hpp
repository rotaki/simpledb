#pragma once

#include <mutex>

#include "query/updatescan.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class temp_table {
public:
  temp_table(transaction *pTx, const schema &pSch);
  std::shared_ptr<update_scan> open();
  std::string table_name();
  layout get_layout();
  std::string next_table_name();

private:
  static int mNextTableNum;
  static std::mutex mMutex;
  transaction *mTx;
  std::string mTblName;
  layout mLayout;
};
} // namespace simpledb

#pragma once

#include <memory>

#include "metadata/tablemanager.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class view_manager {
public:
  view_manager(bool pIsNew, table_manager *pTM, transaction *pTx);
  void create_view(const std::string &pVName, const std::string &pVDef,
                   transaction *pTx);
  std::string get_view_def(const std::string &pVName, transaction *pTx);

private:
  const int mMaxViewDiff = 100; // max view def chars
  table_manager *mTM;
};
} // namespace simpledb

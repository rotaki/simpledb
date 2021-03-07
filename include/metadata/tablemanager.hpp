#pragma once

#include <memory>

#include "record/layout.hpp"
#include "record/schema.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class table_manager {
public:
  table_manager(bool pIsNew, transaction *pTx);
  void create_table(const std::string &pTblName, const schema &pSch,
                    transaction *pTx);
  layout get_layout(const std::string &pTblName, transaction *pTx);
  const int mMaxName = 16;

private:
  layout mTCatLayout, mFCatLayout;
};
} // namespace simpledb

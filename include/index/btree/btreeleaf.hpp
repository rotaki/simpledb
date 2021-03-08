#pragma once

#include <memory>

#include "file/blockid.hpp"
#include "index/btree/btpage.hpp"
#include "index/btree/direntry.hpp"
#include "record/layout.hpp"
#include "record/rid.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class btree_leaf {
public:
  btree_leaf(transaction *pTx, const block_id &pBlk, const layout &pLt,
             const constant &pSeachKey);
  void close();
  bool next();
  rid get_data_rid();
  void remove(const rid &pDataRid);
  dir_entry insert(const rid &pDataRID);
  bool try_overflow();

private:
  transaction *mTx;
  layout mLt;
  constant mSearchKey;
  std::unique_ptr<bt_page> mContents;
  int mCurrentSlot;
  std::string mFileName;
};
} // namespace simpledb

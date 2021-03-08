#pragma once

#include <memory>
#include <string>

#include "file/blockid.hpp"
#include "index/btree/btpage.hpp"
#include "index/btree/direntry.hpp"
#include "query/constant.hpp"
#include "record/layout.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class btree_dir {
public:
  btree_dir(transaction *pTx, const block_id &pBlk, const layout &pLt);
  void close();
  int search(const constant &pSearchKey);
  void make_new_root(const dir_entry &pDE);
  dir_entry insert(const dir_entry &pDE);

private:
  transaction *mTx;
  layout mLt;
  std::unique_ptr<bt_page> mContents;
  std::string mFileName;

  dir_entry insert_entry(const dir_entry &pDE);
  block_id find_child_block(const constant &pSearchKey);
};
} // namespace simpledb

#pragma once

#include <memory>

#include "file/blockid.hpp"
#include "index/btree/btreeleaf.hpp"
#include "index/index.hpp"
#include "record/layout.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class btree_index : public index {
public:
  btree_index(transaction *pTx, const std::string &pIdxName, const layout &mLt);
  void before_first(const constant &pSearchKey) override;
  bool next() override;
  rid get_data_rid() override;
  void insert(const constant &pDataVal, const rid &pDataRid) override;
  void remove(const constant &pDataVal, const rid &pDataRid) override;
  void close() override;
  static int search_cost(const int &pNumBlocks, const int &pRPB);

private:
  transaction *mTx;
  std::string mLeafTbl;
  layout mDirLayout, mLeafLayout;
  std::unique_ptr<btree_leaf> mLeaf;
  block_id mRootBlk;
};
} // namespace simpledb

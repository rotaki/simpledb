#pragma once

#include <memory>

#include "file/blockid.hpp"
#include "tx/transaction.hpp"
#include "record/layout.hpp"
#include "index/index.hpp"
#include "index/btree/btreeleaf.hpp"

namespace smartdb {
  class btree_index: public index {
  public:
    btree_index(std::shared_ptr<transaction> pTx, const std::string &pIdxName, std::shared_ptr<layout> mLt);
    void before_first(const constant &pSearchKey) override;
    bool next() override;
    rid get_data_rid() override;
    void insert(const constant &pDataVal, const rid &pDataRid) override;
    void remove(const constant &pDataVal, const rid &pDataRid) override;
    void close() override;
    static int search_cost(const int &pNumBlocks, const int &pRPB);
  private:
    std::shared_ptr<transaction> mTx;
    std::string mLeafTbl;
    std::shared_ptr<layout> mDirLayout, mLeafLayout;
    std::shared_ptr<btree_leaf> mLeaf;
    std::shared_ptr<block_id> mRootBlk;
  };
}

#include <cmath>
#include <limits>

#include "index/btree/btreedir.hpp"
#include "index/btree/btreeindex.hpp"

namespace simpledb {
btree_index::btree_index(transaction *pTx, const std::string &pIdxName,
                         const layout &pLeafLayout)
    : mTx(pTx), mLeafTbl(pIdxName + "leaf"), mLeafLayout(pLeafLayout) {

  // deal with the leaves
  if (mTx->size(mLeafTbl) == 0) {
    block_id blk = mTx->append(mLeafTbl);
    bt_page node(mTx, blk, mLeafLayout);
    node.format(blk, -1);
  }

  // deal with the directory
  schema dirSch;
  dirSch.add("block", mLeafLayout.get_schema());
  dirSch.add("dataval", mLeafLayout.get_schema());
  std::string dirTbl = pIdxName + "dir";
  mDirLayout = layout(dirSch);
  mRootBlk = block_id(dirTbl, 0);
  if (mTx->size(dirTbl) == 0) {
    // create new root block
    mTx->append(dirTbl);
    bt_page node(mTx, mRootBlk, mDirLayout);
    node.format(mRootBlk, 0);

    // insert initial directory entry
    int fldType = dirSch.type("dataval");
    constant minVal =
        (fldType == schema::integer ? constant(std::numeric_limits<int>::min())
                                    : constant(""));
    node.insert_dir(0, minVal, 0);
    node.close();
  }
}

void btree_index::before_first(const constant &pSearchKey) {
  close();
  btree_dir root(mTx, mRootBlk, mDirLayout);
  int blkNum = root.search(pSearchKey);
  root.close();
  block_id leafBlk(mLeafTbl, blkNum);
  mLeaf = std::make_unique<btree_leaf>(mTx, leafBlk, mLeafLayout, pSearchKey);
}

bool btree_index::next() { return mLeaf->next(); }

rid btree_index::get_data_rid() { return mLeaf->get_data_rid(); }

void btree_index::insert(const constant &pDataVal, const rid &pDataRID) {
  before_first(pDataVal);
  dir_entry e = mLeaf->insert(pDataRID);
  mLeaf->close();
  if (e.is_null()) {
    return;
  }
  btree_dir root(mTx, mRootBlk, mDirLayout);
  dir_entry e2 = root.insert(e);
  if (e2.is_null()) {
    root.make_new_root(e2);
  }
  root.close();
}

void btree_index::remove(const constant &pDataVal, const rid &pDataRID) {
  before_first(pDataVal);
  mLeaf->remove(pDataRID);
  mLeaf->close();
}

void btree_index::close() {
  if (mLeaf) {
    mLeaf->close();
  }
}

int btree_index::search_cost(int pNumBlocks, int pRPB) {
  return 1 + std::round(std::log(pNumBlocks) / std::log(pRPB));
}
} // namespace simpledb

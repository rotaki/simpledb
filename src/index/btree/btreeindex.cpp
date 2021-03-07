#include <limits>
#include <cmath>

#include "index/btree/btreedir.hpp"
#include "index/btree/btreeindex.hpp"

namespace simpledb {
  btree_index::btree_index(std::shared_ptr<transaction> pTx, const std::string &pIdxName, std::shared_ptr<layout> pLeafLayout):
    mTx(pTx), mLeafLayout(pLeafLayout)
  {
    // deal with leaves
    mLeafTbl = pIdxName + "leaf";
    if (mTx->size(mLeafTbl) == 0) {
      std::shared_ptr<block_id> blk = std::make_shared<block_id>(mTx->append(mLeafTbl));
      std::shared_ptr<bt_page> node(new bt_page(mTx, blk, mLeafLayout));
      node->format(blk, -1);
    }

    // deal with the directory
    std::shared_ptr<schema> dirSch(new schema());
    dirSch->add("block", mLeafLayout->get_schema()); // todo fix
    dirSch->add("dataval", mLeafLayout->get_schema()); // todo fix
    std::string dirTbl = pIdxName + "dir";
    mDirLayout = std::shared_ptr<layout>(new layout(*dirSch)); // todo fix
    mRootBlk = std::shared_ptr<block_id>(new block_id(dirTbl, 0));
    if (mTx->size(dirTbl) == 0) {
      // create new root block
      mTx->append(dirTbl);
      std::shared_ptr<bt_page> node(new bt_page(mTx, mRootBlk, mDirLayout));
      node->format(mRootBlk, 0);
      
      // insert initial directory entry
      int fldType = dirSch->type("dataval");
      constant minVal = (fldType == schema::integer ? constant(std::numeric_limits<int>::min()) : constant(""));
      node->insert_dir(0, minVal, 0);
      node->close();
    }
  }

  void btree_index::before_first(const constant &pSearchKey) {
    close();
    std::shared_ptr<btree_dir> root(new btree_dir(mTx, mRootBlk, mDirLayout));
    int blkNum = root->search(pSearchKey);
    root->close();
    std::shared_ptr<block_id> leafBlk(new block_id(mLeafTbl, blkNum));
    mLeaf = std::shared_ptr<btree_leaf>(new btree_leaf(mTx, leafBlk, mLeafLayout, pSearchKey));
  }

  bool btree_index::next() {
    return mLeaf->next();
  }

  rid btree_index::get_data_rid() {
    return mLeaf->get_data_rid();
  }

  void btree_index::insert(const constant &pDataVal, const rid &pDataRID) {
    before_first(pDataVal);
    dir_entry e = mLeaf->insert(pDataRID);
    mLeaf->close();
    if (e.is_null()) {
      return;
    }
    std::shared_ptr<btree_dir> root(new btree_dir(mTx, mRootBlk, mDirLayout));
    dir_entry e2 = root->insert(e);
    if (e2.is_null()) {
      root->make_new_root(e2);
    }
    root->close();
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

  int btree_index::search_cost(const int &pNumBlocks, const int &pRPB) {
    return 1 + std::round(std::log(pNumBlocks)/std::log(pRPB));
  }
}

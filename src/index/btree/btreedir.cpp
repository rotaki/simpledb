#include "index/btree/btreedir.hpp"

namespace smartdb {
  btree_dir::btree_dir(std::shared_ptr<transaction> pTx,
                       std::shared_ptr<block_id> pBlk,
                       std::shared_ptr<layout> pLt):
    mTx(pTx), mLt(pLt)
  {
    mContents = std::shared_ptr<bt_page>(new bt_page(mTx, pBlk, mLt));
    mFileName = pBlk->file_name();
  }

  void btree_dir::close() {
    mContents->close();
  }

  int btree_dir::search(const constant &pSearchKey) {
    std::shared_ptr<block_id> childBlk = find_child_block(pSearchKey);
    while (mContents->get_flag() > 0) {
      mContents->close();
      mContents = std::shared_ptr<bt_page>(new bt_page(mTx, childBlk, mLt));
      childBlk = find_child_block(pSearchKey);
    }
    return childBlk->number();
  }

  void btree_dir::make_new_root(const dir_entry &pDE) {
    constant firstVal = mContents->get_data_val(0);
    int level = mContents->get_flag();
    std::shared_ptr<block_id> newBlk = mContents->split(0, level);
    dir_entry oldRoot = dir_entry(firstVal, newBlk->number());
    insert_entry(oldRoot);
    insert_entry(pDE);
    mContents->set_flag(level+1);
  }

  dir_entry btree_dir::insert(const dir_entry &pDE) {
    if (mContents->get_flag() == 0) {
      return insert_entry(pDE);
    }
    std::shared_ptr<block_id> childBlk = find_child_block(pDE.data_val());
    std::shared_ptr<btree_dir> child = std::shared_ptr<btree_dir>(new btree_dir(mTx, childBlk, mLt));
    dir_entry myEntry = child->insert(pDE);
    child->close();
    if (myEntry.is_null()) {
      return dir_entry();
    } else {
      return insert_entry(myEntry);
    }
  }

  dir_entry btree_dir::insert_entry(const dir_entry &pDE) {
    int newSlot = 1 + mContents->find_slot_before(pDE.data_val());
    mContents->insert_dir(newSlot, pDE.data_val(), pDE.block_number());
    if (!mContents->is_full()) {
      return dir_entry();
    }
    // else page is full, so split it
    int level = mContents->get_flag();
    int splitPos = mContents->get_num_recs() / 2;
    constant splitVal = mContents->get_data_val(splitPos);
    std::shared_ptr<block_id> newBlk = mContents->split(splitPos, level);
    return dir_entry(splitVal, newBlk->number());
  }

  std::shared_ptr<block_id> btree_dir::find_child_block(const constant &pSearchKey) {
    int slot = mContents->find_slot_before(pSearchKey);
    if (mContents->get_data_val(slot+1) == pSearchKey) {
      slot++;
    }
    int blkNum = mContents->get_child_num(slot);
    return std::shared_ptr<block_id>(new block_id(mFileName, blkNum));
  }
  
}

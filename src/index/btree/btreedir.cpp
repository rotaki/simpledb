#include "index/btree/btreedir.hpp"

namespace simpledb {
btree_dir::btree_dir(transaction *pTx, const block_id &pBlk, const layout &pLt)
    : mTx(pTx), mLt(pLt) {
  mContents = std::make_unique<bt_page>(mTx, pBlk, mLt);
  mFileName = pBlk.file_name();
}

void btree_dir::close() { mContents->close(); }

int btree_dir::search(const constant &pSearchKey) {
  block_id childBlk = find_child_block(pSearchKey);
  while (mContents->get_flag() > 0) {
    mContents->close();
    mContents = std::make_unique<bt_page>(mTx, childBlk, mLt);
    childBlk = find_child_block(pSearchKey);
  }
  return childBlk.number();
}

void btree_dir::make_new_root(const dir_entry &pDE) {
  constant firstVal = mContents->get_data_val(0);
  int level = mContents->get_flag();
  block_id newBlk = mContents->split(0, level);
  dir_entry oldRoot = dir_entry(firstVal, newBlk.number());
  insert_entry(oldRoot);
  insert_entry(pDE);
  mContents->set_flag(level + 1);
}

dir_entry btree_dir::insert(const dir_entry &pDE) {
  if (mContents->get_flag() == 0) {
    return insert_entry(pDE);
  }
  block_id childBlk = find_child_block(pDE.data_val());
  btree_dir child(mTx, childBlk, mLt);
  dir_entry myEntry = child.insert(pDE);
  child.close();
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
  block_id newBlk = mContents->split(splitPos, level);
  return dir_entry(splitVal, newBlk.number());
}

block_id btree_dir::find_child_block(const constant &pSearchKey) {
  int slot = mContents->find_slot_before(pSearchKey);
  if (mContents->get_data_val(slot + 1) == pSearchKey) {
    slot++;
  }
  int blkNum = mContents->get_child_num(slot);
  return block_id(mFileName, blkNum);
}

} // namespace simpledb

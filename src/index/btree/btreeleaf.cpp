#include "index/btree/btreeleaf.hpp"

namespace simpledb {
  btree_leaf::btree_leaf(std::shared_ptr<transaction> pTx, std::shared_ptr<block_id> pBlk, std::shared_ptr<layout> pLt, const constant &pSearchKey):
    mTx(pTx), mLt(pLt), mSearchKey(pSearchKey) {
    mContents = std::shared_ptr<bt_page>(new bt_page(mTx, pBlk, mLt));
    mCurrentSlot = mContents->find_slot_before(mSearchKey);
    mFileName = pBlk->file_name();
  }

  void btree_leaf::close() {
    return mContents->close();
  }

  bool btree_leaf::next() {
    mCurrentSlot++;
    if (mCurrentSlot >= mContents->get_num_recs()) {
      return try_overflow();
    } else if (mContents->get_data_val(mCurrentSlot) == mSearchKey) {
      return true;
    } else {
      return try_overflow();
    }
  }


  rid btree_leaf::get_data_rid() {
    return mContents->get_data_rid(mCurrentSlot);
  }

  void btree_leaf::remove(const rid &pDataRID) {
    while (next()) {
      if (get_data_rid() == pDataRID) {
        mContents->remove(mCurrentSlot);
        return;
      }
    }
  }

  dir_entry btree_leaf::insert(const rid &pDataRID) {
    if (mContents->get_flag() >= 0 && mContents->get_data_val(0) > mSearchKey) {
      constant firstVal = mContents->get_data_val(0);
      std::shared_ptr<block_id> newBlk = mContents->split(0, mContents->get_flag());
      mCurrentSlot = 0;
      mContents->set_flag(-1);
      mContents->insert_leaf(mCurrentSlot, mSearchKey, pDataRID);
      return dir_entry(firstVal, newBlk->number());
    }

    mCurrentSlot++;
    mContents->insert_leaf(mCurrentSlot, mSearchKey, pDataRID);
    if (!mContents->is_full()) {
      return dir_entry();
    }

    // else page is full, so split it
    constant firstKey = mContents->get_data_val(0);
    constant lastKey = mContents->get_data_val(mContents->get_num_recs()-1);
    if (lastKey == firstKey) {
      // create an overflow block to hold all but the first record
      std::shared_ptr<block_id> newBlk = mContents->split(1, mContents->get_flag());
      mContents->set_flag(newBlk->number());
      return dir_entry();
    } else {
      int splitPos = mContents->get_num_recs() / 2;
      constant splitKey = mContents->get_data_val(splitPos);
      if (splitKey == firstKey) {
        // move right, looking for the next key
        while (mContents->get_data_val(splitPos-1) == splitKey) {
          splitPos++;
        }
        splitKey = mContents->get_data_val(splitPos);
      } else {
        // move left, looking for first entry having that key
        while (mContents->get_data_val(splitPos-1) == splitKey) {
          splitPos--;
        }
      }
      std::shared_ptr<block_id> newBlk = mContents->split(splitPos, -1);
      return dir_entry(splitKey, newBlk->number());
    }
  }

  bool btree_leaf::try_overflow() {
    constant firstKey = mContents->get_data_val(0);
    int flag = mContents->get_flag();
    if (mSearchKey != firstKey || flag < 0) {
      return false;
    }
    mContents->close();
    std::shared_ptr<block_id> nextBlk(new block_id(mFileName, flag));
    mContents = std::shared_ptr<bt_page>(new bt_page(mTx, nextBlk, mLt));
    mCurrentSlot = 0;
    return true;
  }
}

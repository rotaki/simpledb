#include "multibuffer/chunkscan.hpp"
#include "record/schema.hpp"

namespace simpledb {
chunk_scan::chunk_scan(transaction *pTx, const std::string &pFileName,
                       const layout &pLt, int pStartBNum, int pEndBNum)
    : mTx(pTx), mFileName(pFileName), mLt(pLt), mStartBNum(pStartBNum),
      mEndBNum(pEndBNum) {
  for (int i = mStartBNum; i <= mEndBNum; i++) {
    block_id blk(mFileName, i);
    auto rP = std::make_unique<record_page>(mTx, blk, mLt);
    mBuffs.emplace_back(std::move(rP));
  }
  move_to_block(mStartBNum);
}

void chunk_scan::before_first() { move_to_block(mStartBNum); }

void chunk_scan::close() {
  for (int i = 0; i < mBuffs.size(); i++) {
    block_id blk(mFileName, mStartBNum + i);
    mTx->unpin(blk);
  }
}

bool chunk_scan::next() {
  mCurrentSlot = mRP->next_after(mCurrentSlot);
  while (mCurrentSlot < 0) {
    if (mCurrentBNum == mEndBNum) {
      return false;
    }
    move_to_block(mRP->block().number() + 1);
    mCurrentSlot = mRP->next_after(mCurrentSlot);
  }
  return true;
}

int chunk_scan::get_int(const std::string &pFldName) {
  return mRP->get_int(mCurrentSlot, pFldName);
}

std::string chunk_scan::get_string(const std::string &pFldName) {
  return mRP->get_string(mCurrentSlot, pFldName);
}

constant chunk_scan::get_val(const std::string &pFldName) {
  if (mLt.get_schema().type(pFldName) == schema::integer) {
    return constant(get_int(pFldName));
  } else {
    return constant(get_string(pFldName));
  }
}

bool chunk_scan::has_field(const std::string &pFldName) {
  return mLt.get_schema().has_field(pFldName);
}

void chunk_scan::move_to_block(int pBlkNum) {
  mCurrentBNum = pBlkNum;
  mRP = mBuffs[mCurrentBNum - mStartBNum].get();
  mCurrentSlot = -1;
}
} // namespace simpledb

#include "multibuffer/multibufferproductscan.hpp"
#include "multibuffer/bufferneeds.hpp"
#include "multibuffer/chunkscan.hpp"
#include "query/productscan.hpp"

namespace simpledb {
multibuffer_product_scan::multibuffer_product_scan(
    transaction *pTx, const std::shared_ptr<scan> &pLhsScan,
    const std::string &pFileName, const layout &pLt)
    : mTx(pTx), mLhsScan(pLhsScan), mFileName(pFileName), mLt(pLt) {
  mFileSize = mTx->size(mFileName);
  int available = mTx->available_buffers();
  mChunkSize = buffer_needs::best_factor(available, mFileSize);

  mNextBlkNum = 0;
  use_next_chunk();
}

void multibuffer_product_scan::before_first() {
  mNextBlkNum = 0;
  use_next_chunk();
}

bool multibuffer_product_scan::next() {
  while (!mProdScan->next()) {
    if (!use_next_chunk()) {
      return false;
    }
  }
  return true;
}

void multibuffer_product_scan::close() { mProdScan->close(); }

constant multibuffer_product_scan::get_val(const std::string &pFldName) {
  return mProdScan->get_val(pFldName);
}

int multibuffer_product_scan::get_int(const std::string &pFldName) {
  return mProdScan->get_int(pFldName);
}

std::string multibuffer_product_scan::get_string(const std::string &pFldName) {
  return mProdScan->get_string(pFldName);
}

bool multibuffer_product_scan::has_field(const std::string &pFldName) {
  return mProdScan->has_field(pFldName);
}

bool multibuffer_product_scan::use_next_chunk() {
  if (mRhsScan) {
    mRhsScan->close();
  }
  if (mNextBlkNum >= mFileSize) {
    return false;
  }
  int end = mNextBlkNum + mChunkSize - 1;
  if (end >= mFileSize) {
    end = mFileSize - 1;
  }
  mRhsScan = std::static_pointer_cast<scan>(
      std::make_shared<chunk_scan>(mTx, mFileName, mLt, mNextBlkNum, end));
  mLhsScan->before_first();
  mProdScan = std::static_pointer_cast<scan>(
      std::make_shared<product_scan>(mLhsScan, mRhsScan));
  mNextBlkNum = end + 1;
  return true;
}
} // namespace simpledb

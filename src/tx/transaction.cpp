#include <iostream>

#include "tx/transaction.hpp"
#include "utils/logger.hpp"

namespace simpledb {
int transaction::mNextTxNum = 0;
std::mutex transaction::mMutex;

transaction::transaction(file_manager *pFM, log_manager *pLM,
                         buffer_manager *pBM)
    : mFM(pFM), mLM(pLM), mBM(pBM) {
  mTxNum = next_tx_number();
  mRM = std::make_unique<recovery_manager>(this, mTxNum, mLM, mBM);
  mCM = std::make_unique<concurrency_manager>();
  mBL = std::make_unique<buffer_list>(mBM);
}

void transaction::commit() {
  mRM->commit();
  mCM->release();
  mBL->unpin_all();
  std::cout << "transaction " + std::to_string(mTxNum) + " committed"
            << std::endl;
}

void transaction::rollback() {
  mRM->rollback();
  mCM->release();
  mBL->unpin_all();
  std::cout << "transaction " + std::to_string(mTxNum) + " rolled back"
            << std::endl;
}

void transaction::recover() {
  mBM->flush_all(mTxNum);
  mRM->recover();
}

void transaction::pin(const block_id &pBlockId) { mBL->pin(pBlockId); }

void transaction::unpin(const block_id &pBlockId) { mBL->unpin(pBlockId); }

int transaction::get_int(const block_id &pBlockId, int pOffset) {
  mCM->slock(pBlockId);
  buffer *buff = mBL->get_buffer(pBlockId);
  return buff->contents()->get_int(pOffset);
}

std::string transaction::get_string(const block_id &pBlockId, int pOffset) {
  mCM->slock(pBlockId);
  buffer *buff = mBL->get_buffer(pBlockId);
  return buff->contents()->get_string(pOffset);
}

void transaction::set_int(const block_id &pBlockId, int pOffset, int pVal,
                          bool pOkToLog) {
  mCM->xlock(pBlockId);
  buffer *buff = mBL->get_buffer(pBlockId);
  int lsn = -1;
  if (pOkToLog) {
    lsn = mRM->set_int(buff, pOffset, pVal);
  }
  page *p = buff->contents();
  p->set_int(pOffset, pVal);
  buff->set_modified(mTxNum, lsn);
}

void transaction::set_string(const block_id &pBlockId, int pOffset,
                             const std::string &pVal, bool pOkToLog) {
  mCM->xlock(pBlockId);
  buffer *buff = mBL->get_buffer(pBlockId);
  int lsn = -1;
  if (pOkToLog) {
    lsn = mRM->set_string(buff, pOffset, pVal);
  }
  page *p = buff->contents();
  p->set_string(pOffset, pVal);
  buff->set_modified(mTxNum, lsn);
}

int transaction::size(const std::string &pFileName) {
  block_id dummyBlockId(pFileName, mEndOfFile);
  mCM->slock(dummyBlockId);
  return mFM->length(pFileName);
}

block_id transaction::append(const std::string &pFileName) {
  block_id dummyBlockId(pFileName, mEndOfFile);
  mCM->xlock(dummyBlockId);
  return mFM->append(pFileName);
}

int transaction::block_size() { return mFM->block_size(); }

int transaction::available_buffers() { return mBM->available(); }

int transaction::num() const { return mTxNum; }

int transaction::next_tx_number() {
  std::unique_lock<std::mutex> lock(mMutex);
  mNextTxNum++;
  std::cout << "new transaction: " << std::to_string(mNextTxNum) << std::endl;
  return mNextTxNum;
}
}; // namespace simpledb

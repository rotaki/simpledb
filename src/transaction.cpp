#include <iostream>

#include "transaction.hpp"
#include "logger.hpp"

namespace smartdb {
  int transaction::mNextTxNum = 0;
  std::mutex transaction::mMutex;
  
  transaction::transaction(std::shared_ptr<file_manager> pFM,
                           std::shared_ptr<log_manager> pLM,
                           std::shared_ptr<buffer_manager> pBM):
    mFM(pFM),mLM(pLM), mBM(pBM) {}

  std::shared_ptr<transaction> transaction::create(std::shared_ptr<file_manager> pFM,
                                             std::shared_ptr<log_manager> pLM,
                                             std::shared_ptr<buffer_manager> pBM) {
    std::shared_ptr<transaction> tx(new transaction(pFM, pLM, pBM));
    tx->start();
    return tx;
  }

  void transaction::start() {
    mTxNum = next_tx_number();
    mRM = std::shared_ptr<recovery_manager>(new recovery_manager(shared_from_this(), mTxNum, mLM, mBM));
    mCM = std::shared_ptr<concurrency_manager>(new concurrency_manager);
    mBL = std::shared_ptr<buffer_list>(new buffer_list(mBM));
  }

  void transaction::commit() {
    mRM->commit();
    mCM->release();
    mBL->unpin_all();
    std::cout << "transaction " + std::to_string(mTxNum) + " committed" << std::endl;
  }

  void transaction::rollback() {
    mRM->rollback();
    mCM->release();
    mBL->unpin_all();
    std::cout << "transaction " + std::to_string(mTxNum) + " rolled back" << std::endl;
  }

  void transaction::recover() {
    mBM->flush_all(mTxNum);
    mRM->recover();
  }

  void transaction::pin(std::shared_ptr<block_id> pBlockId) {
    mBL->pin(pBlockId);
  }

  void transaction::unpin(std::shared_ptr<block_id> pBlockId) {
    mBL->unpin(pBlockId);
  }

  int transaction::get_int(std::shared_ptr<block_id> pBlockId, const int& pOffset) {
    mCM->slock(*pBlockId);
    std::shared_ptr<buffer> buff = mBL->get_buffer(pBlockId);
    return buff->contents()->get_int(pOffset);
  }

  std::string transaction::get_string(std::shared_ptr<block_id> pBlockId, const int& pOffset) {
    mCM->slock(*pBlockId);
    std::shared_ptr<buffer>buff = mBL->get_buffer(pBlockId);
    return buff->contents()->get_string(pOffset);
  }

  void transaction::set_int(std::shared_ptr<block_id> pBlockId,
                            const int& pOffset,
                            const int& pVal,
                            const bool& pOkToLog) {
    mCM->xlock(*pBlockId);
    std::shared_ptr<buffer> buff = mBL->get_buffer(pBlockId);
    int lsn = -1;
    if (pOkToLog) {
      lsn = mRM->set_int(buff, pOffset, pVal);
    }
    std::shared_ptr<page> p = buff->contents();
    p->set_int(pOffset, pVal);
    buff->set_modified(mTxNum, lsn);
  }

  void transaction::set_string(std::shared_ptr<block_id> pBlockId,
                               const int &pOffset,
                               const std::string &pVal,
                               const bool &pOkToLog) {
    mCM->xlock(*pBlockId);
    std::shared_ptr<buffer> buff = mBL->get_buffer(pBlockId);
    int lsn = -1;
    if (pOkToLog) {
      lsn = mRM->set_string(buff, pOffset, pVal);
    }
    std::shared_ptr<page> p = buff->contents();
    p->set_string(pOffset, pVal);
    buff->set_modified(mTxNum, lsn);
  }

  int transaction::size(const std::string &pFileName) {
    std::shared_ptr<block_id> dummyBlockId(new block_id(pFileName, mEndOfFile));
    mCM->slock(*dummyBlockId);
    return mFM->length(pFileName);
  }

  std::shared_ptr<block_id> transaction::append(const std::string &pFileName) {
    std::shared_ptr<block_id> dummyBlockId(new block_id(pFileName, mEndOfFile));
    mCM->xlock(*dummyBlockId);
    return mFM->append(pFileName);
  }

  int transaction::block_size() {
    return mFM->block_size();
  }

  int transaction::available_buffers() {
    return mBM->available();
  }
  
  int transaction::next_tx_number() {
    std::unique_lock<std::mutex> lock(mMutex);
    mNextTxNum++;
    std::cout << "new transaction: " << std::to_string(mNextTxNum) << std::endl;
    return mNextTxNum;
  }
};

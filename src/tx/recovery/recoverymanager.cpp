#include <algorithm>

#include "tx/recovery/logrecord.hpp"
#include "tx/recovery/recoverymanager.hpp"

namespace simpledb {
recovery_manager::recovery_manager(transaction *pTx, int pTxNum,
                                   log_manager *pLM, buffer_manager *pBM)
    : mTx(pTx), mTxNum(pTxNum), mLM(pLM), mBM(pBM) {
  start_record::write_to_log(mLM, mTxNum);
}

void recovery_manager::commit() {
  mBM->flush_all(mTxNum);
  int lsn = commit_record::write_to_log(mLM, mTxNum);
  mLM->flush(lsn);
}

void recovery_manager::rollback() {
  do_rollback();
  mBM->flush_all(mTxNum);
  int lsn = rollback_record::write_to_log(mLM, mTxNum);
  mLM->flush(lsn);
}

void recovery_manager::recover() {
  do_recover();
  mBM->flush_all(mTxNum);
  int lsn = checkpoint_record::write_to_log(mLM);
  mLM->flush(lsn);
}

int recovery_manager::set_int(buffer *pBuff, int pOffset, int pNewVal) {
  int oldVal = pBuff->contents()->get_int(pOffset);
  block_id blockId = pBuff->block();
  return set_int_record::write_to_log(mLM, mTxNum, blockId, pOffset, oldVal);
}

int recovery_manager::set_string(buffer *pBuff, int pOffset,
                                 const std::string &pNewVal) {
  std::string oldVal = pBuff->contents()->get_string(pOffset);
  block_id blockId = pBuff->block();
  return set_string_record::write_to_log(mLM, mTxNum, blockId, pOffset, oldVal);
}

void recovery_manager::do_rollback() {
  log_manager::log_iterator iter = mLM->iterator();
  while (iter.has_next()) {
    std::vector<char> byteVec = iter.next();
    auto rec = log_record::create_log_record(byteVec);
    if (rec->tx_number() == mTxNum) {
      if (rec->op() == log_record::start) {
        return;
      } else {
        rec->undo(mTx);
      }
    }
  }
}

void recovery_manager::do_recover() {
  std::vector<int> finished_txs;
  log_manager::log_iterator iter = mLM->iterator();
  while (iter.has_next()) {
    std::vector<char> byteVec = iter.next();
    auto rec = log_record::create_log_record(byteVec);
    if (rec->op() == log_record::checkpoint) {
      return;
    }
    if (rec->op() == log_record::commit || rec->op() == log_record::rollback) {
      finished_txs.emplace_back(rec->tx_number());
    } else if (std::find(finished_txs.begin(), finished_txs.end(),
                         rec->tx_number()) == finished_txs.end()) {
      rec->undo(mTx);
    }
  }
}
} // namespace simpledb

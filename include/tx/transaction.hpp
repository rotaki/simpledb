#pragma once

#include <string>

#include "buffer/buffermanager.hpp"
#include "file/filemanager.hpp"
#include "tx/bufferlist.hpp"
#include "tx/concurrency/concurrencymanager.hpp"
#include "tx/recovery/recoverymanager.hpp"

namespace simpledb {
class transaction {
public:
  transaction(file_manager *pFM, log_manager *pLM, buffer_manager *pBM);

  void commit();
  void rollback();
  void recover();
  void pin(const block_id &pBlockId);
  void unpin(const block_id &pBlockId);
  int get_int(const block_id &pBlockId, const int &pOffset);
  std::string get_string(const block_id &pBlockId, const int &pOffset);
  void set_int(const block_id &pBlockId, const int &pOffset, const int &pVal,
               const bool &pOkToLog);
  void set_string(const block_id &pBlockId, const int &pOffset,
                  const std::string &pVal, const bool &pOkToLog);
  int size(const std::string &pFileName);
  block_id append(const std::string &pFileName);
  int block_size();
  int available_buffers();

private:
  static int mNextTxNum;
  static std::mutex mMutex;
  const int mEndOfFile = -1;
  file_manager *mFM;
  log_manager *mLM;
  buffer_manager *mBM;
  int mTxNum;
  std::unique_ptr<recovery_manager> mRM;
  std::unique_ptr<concurrency_manager> mCM;
  std::unique_ptr<buffer_list> mBL;

  static int next_tx_number();
};
} // namespace simpledb

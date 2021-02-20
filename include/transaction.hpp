#pragma once

#include <iostream>
#include <string>

#include "recoverymanager.hpp"
#include "concurrencymanager.hpp"
#include "buffermanager.hpp"
#include "filemanager.hpp"
#include "bufferlist.hpp"

namespace smartdb {
  class transaction: public std::enable_shared_from_this<transaction> {
  public:
    static std::shared_ptr<transaction> create(std::shared_ptr<file_manager> pFM,
                                               std::shared_ptr<log_manager> pLM,
                                               std::shared_ptr<buffer_manager> pBM);
    void commit();
    void rollback();
    void recover();
    void pin(std::shared_ptr<block_id> pBlockId);
    void unpin(std::shared_ptr<block_id> pBlockId);
    int get_int(std::shared_ptr<block_id> pBlockId,
                const int &pOffset);
    std::string get_string(std::shared_ptr<block_id> pBlockId,
                           const int &pOffset);
    void set_int(std::shared_ptr<block_id> pBlockId,
                 const int &pOffset,
                 const int &pVal,
                 const bool &pOkToLog);
    void set_string(std::shared_ptr<block_id> pBlockId,
                    const int &pOffset,
                    const std::string &pVal,
                    const bool &pOkToLog);
    int size(const std::string &pFileName);
    std::shared_ptr<block_id> append(const std::string &pFileName);
    int block_size();
    int available_buffers();
  private:
    static int mNextTxNum;      // todo dont forget to instantiate
    static std::mutex mMutex;
    const int mEndOfFile = -1;
    std::shared_ptr<file_manager> mFM;
    std::shared_ptr<log_manager> mLM;
    std::shared_ptr<buffer_manager> mBM;
    int mTxNum;
    std::shared_ptr<recovery_manager> mRM;
    std::shared_ptr<concurrency_manager> mCM;
    std::shared_ptr<buffer_list> mBL;

    transaction(std::shared_ptr<file_manager> pFM,
                std::shared_ptr<log_manager> pLM,
                std::shared_ptr<buffer_manager> pBM);
    void start();

    static int next_tx_number();
  };
}

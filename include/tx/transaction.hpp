#pragma once

#include <string>

#include "tx/recovery/recoverymanager.hpp"
#include "tx/concurrency/concurrencymanager.hpp"
#include "buffer/buffermanager.hpp"
#include "file/filemanager.hpp"
#include "tx/bufferlist.hpp"

namespace smartdb {
  class transaction {
  public:
    transaction(file_manager* pFM,
                log_manager* pLM,
                buffer_manager* pBM);

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
    static int mNextTxNum;
    static std::mutex mMutex;
    const int mEndOfFile = -1;
    file_manager* mFM;
    log_manager* mLM;
    buffer_manager* mBM;
    int mTxNum;
    std::shared_ptr<recovery_manager> mRM;
    std::shared_ptr<concurrency_manager> mCM;
    std::shared_ptr<buffer_list> mBL;


    static int next_tx_number();
  };
}

#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>

#include "filemanager.hpp"
#include "logmanager.hpp"

namespace smartdb {
  class buffer {
  public:
    buffer(std::shared_ptr<file_manager> pFileManager, std::shared_ptr<log_manager> pLogManager);
    std::shared_ptr<page> contents();
    std::shared_ptr<block_id> block();
    void set_modified(int pTxNum, int pLSN);
    bool is_pinned();
    int modifying_tx();
    void assign_to_block(std::shared_ptr<block_id> pBlockId);
    void flush();
    void pin();
    void unpin();
    
  private:
    std::shared_ptr<file_manager> mFileManager;
    std::shared_ptr<log_manager> mLogManager;
    std::shared_ptr<page> mContents;
    std::shared_ptr<block_id> mBlockId = std::shared_ptr<block_id>(nullptr);
    int mPins = 0;
    int mTxNum = -1;            // transaction that touched the buffer last
    int mLSN = -1;
  };

  class buffer_manager {
  public:
    buffer_manager(std::shared_ptr<file_manager> pFileManager,
                   std::shared_ptr<log_manager> pLogManager,
                   const int &pNumBuffs);
    
    int available();
    void flush_all(const int &pTxNum);
    void unpin(std::shared_ptr<buffer> pBuff);
    std::shared_ptr<buffer> pin(std::shared_ptr<block_id> pBlockId);
    
  private:
    std::vector<std::shared_ptr<buffer>> mBufferPool;
    int mNumAvailable;          // number of unpinned buffers
    const int mMaxTime = 10000;
    std::mutex mMutex;
    std::condition_variable mCondVar;

    bool waiting_too_long(std::chrono::time_point<std::chrono::high_resolution_clock> pStartTime);
    std::shared_ptr<buffer> try_to_pin(std::shared_ptr<block_id> pBlockId);
    std::shared_ptr<buffer> find_existing_buffer(std::shared_ptr<block_id> pBlockId);
    std::shared_ptr<buffer> choose_unpinned_buffer();
  };
}

#include <chrono>
#include <stdexcept>
#include <iostream>

#include "buffermanager.hpp"


namespace smartdb {
  buffer::buffer(std::shared_ptr<file_manager> pFileManager, std::shared_ptr<log_manager> pLogManager):
    mFileManager(pFileManager), mLogManager(pLogManager)
  {
    mContents = std::shared_ptr<page>(new page(mFileManager->block_size()));
  }

  std::shared_ptr<page> buffer::contents() {
    return mContents;
  }

  std::shared_ptr<block_id> buffer::block() {
    return mBlockId;
  }

  void buffer::set_modified(int pTxNum, int pLSN) {
    mTxNum = pTxNum;
    if (pLSN >= 0) mLSN = pLSN;
  }

  bool buffer::is_pinned() {
    return mPins > 0;
  }

  int buffer::modifying_tx() {
    return mTxNum;
  }

  void buffer::assign_to_block(std::shared_ptr<block_id> pBlockId) {
    flush();
    mBlockId = pBlockId;
    mFileManager->read(*mBlockId, *mContents);
    mPins = 0;
  }

  void buffer::flush() {
    if (mTxNum >= 0) {
      mLogManager->flush(mLSN);
      mFileManager->write(*mBlockId, *mContents);
      mTxNum = -1;
    }
  }

  void buffer::pin() {
    mPins++;
  }

  void buffer::unpin() {
    mPins--;
  }

  buffer_manager::buffer_manager(std::shared_ptr<file_manager> pFileManager,
                                 std::shared_ptr<log_manager> pLogManager,
                                 const int &pNumBuffs) {
    mNumAvailable = pNumBuffs;
    for (int i = 0; i < pNumBuffs; i++) {
      mBufferPool.emplace_back(std::shared_ptr<buffer>(new buffer(pFileManager, pLogManager)));
    }
  }

  int buffer_manager::available() {
    std::unique_lock<std::mutex> lock(mMutex);
    return mNumAvailable;
  }

  void buffer_manager::flush_all(const int &pTxNum) {
    std::unique_lock<std::mutex> lock(mMutex);
    for (auto &buff: mBufferPool) {
      if (buff->modifying_tx() == pTxNum) {
        buff->flush();
      }
    }
  }

  void buffer_manager::unpin(std::shared_ptr<buffer> pBuff) {
    std::unique_lock<std::mutex> lock(mMutex);
    pBuff->unpin();
    if (!pBuff->is_pinned()) {
      mNumAvailable++;
      mCondVar.notify_all();             // todo
    }
  }

  std::shared_ptr<buffer> buffer_manager::pin(std::shared_ptr<block_id> pBlockId) {
    std::unique_lock<std::mutex> lock(mMutex);
    auto start = std::chrono::high_resolution_clock::now();
    std::shared_ptr<buffer> buff = try_to_pin(pBlockId);
    // while buffer is null and not have waited too long
    while (!buff && !waiting_too_long(start)) {
      mCondVar.wait_for(lock, std::chrono::milliseconds(mMaxTime));
    }
    if (!buff) {
      throw std::runtime_error("buffer abort exception");
    }
    return buff;
  }

  bool buffer_manager::waiting_too_long(std::chrono::time_point<std::chrono::high_resolution_clock> pStartTime) {
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-pStartTime).count();
    return elapsed > mMaxTime;
  }

  std::shared_ptr<buffer> buffer_manager::try_to_pin(std::shared_ptr<block_id> pBlockId) {
    std::shared_ptr<buffer> buff = find_existing_buffer(pBlockId);
    // if buffer is not found
    if (!buff) {
      buff = choose_unpinned_buffer();
      if (!buff) {
        return buff;
      }
      buff->assign_to_block(pBlockId);
    }
    // if buffer is not pinned
    if (!buff->is_pinned()) {
      mNumAvailable--;
    }
    buff->pin();
    return buff;
  }
  
  std::shared_ptr<buffer> buffer_manager::find_existing_buffer(std::shared_ptr<block_id> pBlockId) {
    for (auto &buff: mBufferPool) {
      std::shared_ptr<block_id> blockId = buff->block();
      if (blockId && blockId->equals(*pBlockId)) {
        return buff;
      }
    }
    return std::shared_ptr<buffer>(nullptr);
  }

  std::shared_ptr<buffer> buffer_manager::choose_unpinned_buffer() {
    for (auto &buff: mBufferPool) {
      if (!buff->is_pinned()) {
        return buff;
      }
    }
    return std::shared_ptr<buffer>(nullptr);
  }
}

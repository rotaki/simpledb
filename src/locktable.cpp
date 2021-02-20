#include "locktable.hpp"

namespace smartdb {
  lock_table::lock_table() {
    
  }
  void lock_table::slock(std::shared_ptr<block_id> pBlockId) {
    std::unique_lock<std::mutex> lock(mMutex);
    auto start = std::chrono::high_resolution_clock::now();
    while (has_xlock(pBlockId) && !waiting_too_long(start)) {
      mCondVar.wait_for(lock, std::chrono::milliseconds(mMaxTime));
    }
    if (has_xlock(pBlockId)) {
      throw std::runtime_error("lock abort exception");
    }
    int val = get_lock_val(pBlockId);
    mLocks[pBlockId] = val+1;
  }

  void lock_table::xlock(std::shared_ptr<block_id> pBlockId) {
    std::unique_lock<std::mutex> lock(mMutex);
    auto start = std::chrono::high_resolution_clock::now();
    while (has_other_slocks(pBlockId) && !waiting_too_long(start)) {
      mCondVar.wait_for(lock, std::chrono::milliseconds(mMaxTime));
    }
    if (has_other_slocks(pBlockId)) {
      throw std::runtime_error("lock abort exception");
    }
    mLocks[pBlockId] = -1;
  }

  void lock_table::unlock(std::shared_ptr<block_id> pBlockId) {
    std::unique_lock<std::mutex> lock(mMutex);
    int val = get_lock_val(pBlockId);
    if (val > 1) {
      mLocks[pBlockId]--;
    } else {
      mLocks.erase(pBlockId);
      mCondVar.notify_all();
    }
  }

  bool lock_table::has_xlock(std::shared_ptr<block_id> pBlockId) {
    return get_lock_val(pBlockId) < 0;
  }

  bool lock_table::has_other_slocks(std::shared_ptr<block_id> pBlockId) {
    return get_lock_val(pBlockId) > 1; 
  }

  bool lock_table::waiting_too_long(std::chrono::time_point<std::chrono::high_resolution_clock> pStartTime) {
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-pStartTime).count();
    return elapsed > mMaxTime;
  }

  int lock_table::get_lock_val(std::shared_ptr<block_id> pBlockId) {
    auto iter = mLocks.find(pBlockId);
    return ((iter == mLocks.end()) ? 0 : iter->second);
  }
}

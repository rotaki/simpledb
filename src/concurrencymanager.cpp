#include "concurrencymanager.hpp"

namespace smartdb {
  // instantiate static member
  lock_table concurrency_manager::mLockTable; 
  
  void concurrency_manager::slock(const block_id &pBlockId) {
    if (mLocks.find(pBlockId) == mLocks.end()) {
      mLockTable.slock(pBlockId);
      mLocks[pBlockId] = "S";
    }
  }

  void concurrency_manager::xlock(const block_id &pBlockId) {
    if (!has_xlock(pBlockId)) {
      slock(pBlockId);
      mLockTable.xlock(pBlockId);
      mLocks[pBlockId] = "X";
    }
  }

  void concurrency_manager::release() {
    for (auto &item: mLocks) {
      mLockTable.unlock(item.first);
    }
    mLocks.clear();
  }

  bool concurrency_manager::has_xlock(const block_id &pBlockId) {
    auto iter = mLocks.find(pBlockId);
    return (iter != mLocks.end()) && (iter->second == "X");
  }
}

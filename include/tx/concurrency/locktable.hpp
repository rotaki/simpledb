#pragma once

#include <chrono>
#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>

#include "file/blockid.hpp"

namespace simpledb {
class lock_table {
public:
  lock_table();
  void slock(const block_id &pBlockId);
  void xlock(const block_id &pBlockId);
  void unlock(const block_id &pBlockId);

private:
  const int mMaxTime = 10000;
  std::map<block_id, int> mLocks;
  std::mutex mMutex;
  std::condition_variable mCondVar;

  bool has_xlock(const block_id &pBlockId);
  bool has_other_slocks(const block_id &pBlockId);
  bool waiting_too_long(
      std::chrono::time_point<std::chrono::high_resolution_clock> pStartTime);
  int get_lock_val(const block_id &pBlockId);
};
} // namespace simpledb

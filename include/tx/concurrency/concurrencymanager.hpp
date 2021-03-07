#pragma once

#include <memory>

#include "file/blockid.hpp"
#include "tx/concurrency/locktable.hpp"

namespace simpledb {
  class concurrency_manager{
  public:
    void slock(const block_id &pBlockId);
    void xlock(const block_id &pBlockId);
    void release();
  private:
    static lock_table mLockTable;
    std::map<block_id, std::string> mLocks;

    bool has_xlock(const block_id &pBlockId);
  };
}

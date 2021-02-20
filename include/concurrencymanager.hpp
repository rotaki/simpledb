#pragma once

#include <memory>

#include "blockid.hpp"
#include "locktable.hpp"

namespace smartdb {
  class concurrency_manager{
  public:
    void slock(std::shared_ptr<block_id> pBlockId);
    void xlock(std::shared_ptr<block_id> pBlockId);
    void release();
  private:
    static lock_table mLockTable;
    std::map<std::shared_ptr<block_id>, std::string> mLocks;

    bool has_xlock(std::shared_ptr<block_id> pBlockId);
  };
}

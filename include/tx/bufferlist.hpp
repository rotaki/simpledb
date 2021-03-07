#pragma once

#include <memory>
#include <map>

#include "buffer/buffermanager.hpp"

namespace simpledb {
  class buffer_list {
  public:
    buffer_list(buffer_manager* pBM);
    buffer* get_buffer(const block_id &pBlockId);
    void pin(const block_id &pBlockId);
    void unpin(const block_id &pBlockId);
    void unpin_all();

  private:
    std::map<block_id, buffer*> mBuffers;
    std::vector<block_id> mPins;
    buffer_manager* mBM;
  };
}

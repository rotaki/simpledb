#pragma once

#include <memory>
#include <map>

#include "buffermanager.hpp"

namespace smartdb {
  class buffer_list {
  public:
    buffer_list(std::shared_ptr<buffer_manager> pBM);
    std::shared_ptr<buffer> get_buffer(std::shared_ptr<block_id> pBlockId);
    void pin(std::shared_ptr<block_id> pBlockId);
    void unpin(std::shared_ptr<block_id> pBlockId);
    void unpin_all();

  private:
    std::map<block_id, std::shared_ptr<buffer>> mBuffers;
    std::vector<block_id> mPins;
    std::shared_ptr<buffer_manager> mBM;
  };
}

#include <algorithm>

#include "bufferlist.hpp"

namespace smartdb {
  buffer_list::buffer_list(std::shared_ptr<buffer_manager> pBM): mBM(pBM) {}

  std::shared_ptr<buffer> buffer_list::get_buffer(std::shared_ptr<block_id> pBlockId) 
  {
    return mBuffers.at(*pBlockId);
  }

  void buffer_list::pin(std::shared_ptr<block_id> pBlockId) {
    std::shared_ptr<buffer> buff = mBM->pin(pBlockId);
    mBuffers[*pBlockId] = buff;
    mPins.emplace_back(*pBlockId);
  }

  void buffer_list::unpin(std::shared_ptr<block_id> pBlockId) {
    std::shared_ptr<buffer> buff = mBuffers.at(*pBlockId);
    mBM->unpin(buff);
    auto iter = std::find(mPins.begin(), mPins.end(), *pBlockId);
    if (iter != mPins.end()) {
      mPins.erase(iter);
    }
    iter = std::find(mPins.begin(), mPins.end(), *pBlockId);
    if (iter == mPins.end()) {
      mBuffers.erase(*pBlockId);
    }
  }

  void buffer_list::unpin_all() {
    for (auto &pBlockId: mPins) {
      std::shared_ptr<buffer> buff = mBuffers.at(pBlockId);
      mBM->unpin(buff);
    }
    mBuffers.clear();
    mPins.clear();
  }
}

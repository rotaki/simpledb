#include <algorithm>

#include "tx/bufferlist.hpp"

namespace simpledb {
buffer_list::buffer_list(buffer_manager *pBM) : mBM(pBM) {}

buffer *buffer_list::get_buffer(const block_id &pBlockId) {
  return mBuffers.at(pBlockId);
}

void buffer_list::pin(const block_id &pBlockId) {
  buffer *buff = mBM->pin(pBlockId);
  mBuffers[pBlockId] = buff;
  mPins.emplace_back(pBlockId);
}

void buffer_list::unpin(const block_id &pBlockId) {
  buffer *buff = mBuffers.at(pBlockId);
  mBM->unpin(buff);
  auto iter = std::find(mPins.begin(), mPins.end(), pBlockId);
  if (iter != mPins.end()) {
    mPins.erase(iter);
  }
  iter = std::find(mPins.begin(), mPins.end(), pBlockId);
  if (iter == mPins.end()) {
    mBuffers.erase(pBlockId);
  }
}

void buffer_list::unpin_all() {
  for (auto &&blockId : mPins) {
    buffer *buff = mBuffers.at(blockId);
    mBM->unpin(buff);
  }
  mBuffers.clear();
  mPins.clear();
}
} // namespace simpledb

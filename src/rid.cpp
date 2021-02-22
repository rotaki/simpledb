#include "rid.hpp"

namespace smartdb {
  rid::rid(const rid &pRID):
    mBlockNum(pRID.mBlockNum), mSlot(pRID.mSlot) {}
  
  rid::rid(const int &pBlockNum, const int &pSlot):
    mBlockNum(pBlockNum), mSlot(pSlot) {}

  int rid::block_number() const {
    return mBlockNum;
  }

  int rid::slot() const {
    return mSlot;
  }

  bool rid::equals(const rid &pRID) const {
    return (mBlockNum == pRID.block_number()) && (mSlot == pRID.slot());
  }

  std::string rid::to_string() const {
    return "[" + std::to_string(mBlockNum) + ", " + std::to_string(mSlot) + "]";
  }
}


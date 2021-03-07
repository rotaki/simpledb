#pragma once

#include <string>

namespace simpledb {
  class rid {
    friend bool operator==(const rid &pLhs, const rid &pRhs);
  public:
    rid(const rid& pRID);
    rid(const int &pBlockNum, const int &pSlot);
    int block_number() const;
    int slot() const;
    bool equals(const rid &pRID) const;
    std::string to_string() const;
  private:
    int mBlockNum;
    int mSlot;
  };

}

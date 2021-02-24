#pragma once

#include <string>

namespace smartdb {
  class rid {
  public:
    rid(const rid& pRID);
    rid(const int &pBlockNum, const int &pSlot);
    int block_number() const;
    int slot() const;
    bool equals(const rid &pRID) const;
    std::string to_string() const;
    bool operator==(const rid &pRID) const;
  private:
    int mBlockNum;
    int mSlot;
  };

}

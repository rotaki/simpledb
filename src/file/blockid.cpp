#include <string>

#include "file/blockid.hpp"

namespace smartdb {
  bool operator==(const block_id &pLhs, const block_id &pRhs) {
    return (pLhs.mFileName == pRhs.mFileName) && (pLhs.mBlockNum == pRhs.mBlockNum);
  }

  bool operator!=(const block_id &pLhs, const block_id &pRhs) {
    if (pLhs == pRhs) {
      return false;
    } else {
      return true;
    }
  }

  bool operator<(const block_id &pLhs, const block_id &pRhs) {
    if (pLhs.mFileName  == pRhs.mFileName) {
      return pLhs.mBlockNum < pRhs.mBlockNum;
    } else {
      return pLhs.mFileName < pRhs.mFileName;
    }
  }

  bool operator>(const block_id &pLhs, const block_id &pRhs) {
    if (pLhs.mFileName  == pRhs.mFileName) {
      return pLhs.mBlockNum > pRhs.mBlockNum;
    } else {
      return pLhs.mFileName > pRhs.mFileName;
    }
  }

  bool operator<=(const block_id &pLhs, const block_id &pRhs) {
    if (pLhs > pRhs) {
      return false;
    } else {
      return true;
    }
  }

  bool operator>=(const block_id &pLhs, const block_id &pRhs) {
    if (pLhs < pRhs) {
      return false;
    } else {
      return true;
    }
  }
  
  block_id::block_id(const std::string &pFileName, const int &pBlockNum):
    mFileName(pFileName), mBlockNum(pBlockNum) {}

  std::string block_id::file_name() const {
    return mFileName;
  }

  int block_id::number() const {
    return mBlockNum;
  }

  bool block_id::equals(const block_id &obj) const {
    return (obj.file_name() == mFileName) && (obj.number() == mBlockNum);
  }

  std::string block_id::to_string() const {
    return mFileName + ", " + std::to_string(mBlockNum);
  }
}

#include <string>

#include "blockid.hpp"

namespace smartdb {
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

  bool block_id::operator<(const block_id &pBlockId) const {
    if (mFileName == pBlockId.file_name()) {
      return mBlockNum < pBlockId.number();
    } else {
      return mFileName < pBlockId.file_name();
    }
  }

  bool block_id::operator==(const block_id &pBlockId) const {
    return equals(pBlockId);
  }
}

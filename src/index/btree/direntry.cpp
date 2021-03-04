#include "index/btree/direntry.hpp"

namespace smartdb {
  dir_entry::dir_entry() {}
  dir_entry::dir_entry(const dir_entry &pDE):
    mDataVal(pDE.mDataVal), mBlockNum(pDE.mBlockNum) {}
  dir_entry::dir_entry(const constant &pDataVal, const int &pBlockNum): mDataVal(pDataVal), mBlockNum(pBlockNum) {}
  dir_entry& dir_entry::operator=(const dir_entry &pDE) {
    if (this != &pDE) {
      mDataVal = pDE.mDataVal;
      mBlockNum = pDE.mBlockNum;
    }
    return *this;
  }
  constant dir_entry::data_val() const {
    return mDataVal;
  }
  int dir_entry::block_number() const {
    return mBlockNum;
  }

  bool dir_entry::is_null() const {
    return mDataVal.is_null();
  }
}


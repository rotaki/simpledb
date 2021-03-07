#pragma once

#include "query/constant.hpp"

namespace simpledb {
class dir_entry {
public:
  dir_entry();
  dir_entry(const dir_entry &pDE);
  dir_entry(const constant &pDataVal, const int &pBlockNum);
  dir_entry &operator=(const dir_entry &pDE);
  constant data_val() const;
  int block_number() const;
  bool is_null() const;

private:
  constant mDataVal;
  int mBlockNum;
};
} // namespace simpledb

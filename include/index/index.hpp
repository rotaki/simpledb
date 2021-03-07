#pragma once

#include "query/constant.hpp"
#include "record/rid.hpp"

namespace simpledb {
class index {
public:
  virtual ~index() {}
  virtual void before_first(const constant &pSearchKey) = 0;
  virtual bool next() = 0;
  virtual rid get_data_rid() = 0;
  virtual void insert(const constant &pDataVal, const rid &pDataRID) = 0;
  virtual void remove(const constant &pDataVal, const rid &pDataRID) = 0;
  virtual void close() = 0;
};
} // namespace simpledb

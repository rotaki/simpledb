#pragma once

#include "constant.hpp"
#include "rid.hpp"

namespace smartdb {
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
}

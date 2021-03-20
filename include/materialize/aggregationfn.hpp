#pragma once

#include <string>

#include "query/constant.hpp"
#include "query/scan.hpp"

namespace simpledb {
class aggregation_fn {
public:
  ~aggregation_fn() {}
  virtual void process_first(scan *pS) = 0;
  virtual void process_next(scan *pS) = 0;
  virtual std::string field_name() = 0;
  virtual constant value() = 0;
};
} // namespace simpledb

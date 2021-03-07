#pragma once

#include <memory>

#include "query/scan.hpp"
#include "record/schema.hpp"

namespace simpledb {
class plan {
public:
  virtual ~plan() {}
  virtual std::shared_ptr<scan> open() = 0;
  virtual int blocks_accessed() = 0;
  virtual int records_output() = 0;
  virtual int distinct_values(const std::string &pFldName) = 0;
  virtual schema get_schema() = 0;
};
} // namespace simpledb

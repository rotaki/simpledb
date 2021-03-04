#pragma once

#include <memory>

#include "record/schema.hpp"
#include "query/scan.hpp"

namespace smartdb {
  class plan {
  public:
    virtual ~plan() {}
    virtual std::shared_ptr<scan> open() = 0;
    virtual int blocks_accessed() = 0;
    virtual int records_output() = 0;
    virtual int distinct_values(const std::string &pFldName) = 0;
    virtual std::shared_ptr<schema> get_schema() = 0;
  };
}

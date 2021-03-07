#pragma once

#include <string>

#include "query/constant.hpp"

namespace simpledb {
  class scan {
  public:
    virtual ~scan() = default;
    virtual void before_first() = 0;
    virtual bool next() = 0;
    virtual int get_int(const std::string &pFldName) = 0;
    virtual std::string get_string(const std::string &pFldName) = 0;
    virtual constant get_val(const std::string &pFldName) = 0;
    virtual bool has_field(const std::string &pFldName) = 0;
    virtual void close() = 0;
  };
}

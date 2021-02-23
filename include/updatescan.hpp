#pragma once

#include <string>

#include "scan.hpp"
#include "constant.hpp"
#include "rid.hpp"

namespace smartdb {
  class update_scan: public scan {
  public:
    virtual ~update_scan() = default;
    virtual void set_int(const std::string &pFldName, const int &pVal) = 0;
    virtual void set_string(const std::string &pFldName, const std::string &pVal) = 0;
    virtual void set_val(const std::string &pFldName, const constant &pVal) = 0;
    virtual void insert() = 0;
    virtual void remove() = 0;
    virtual rid get_rid() = 0;
    virtual void move_to_rid(const rid &pRID) = 0;
  };
}

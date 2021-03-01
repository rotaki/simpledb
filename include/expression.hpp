#pragma once

#include <memory>
#include <string>

#include "constant.hpp"
#include "scan.hpp"
#include "schema.hpp"

namespace smartdb {
  class expression {
  public:
    expression();
    expression(const expression &pE);
    expression(const constant &pVal);
    expression(const std::string &pFldName);
    expression& operator=(const expression &pE);
    bool is_field_name() const;
    constant as_constant() const;
    std::string as_field_name() const;
    constant evaluate(std::shared_ptr<scan> pS) const;
    bool applies_to(std::shared_ptr<schema> pSch) const;
    std::string to_string() const;
  private:
    constant mVal;
    std::string mFldName;
  };
}

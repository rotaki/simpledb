#pragma once

#include <memory>
#include <string>

#include "query/constant.hpp"
#include "query/scan.hpp"
#include "record/schema.hpp"

namespace simpledb {
class expression {
public:
  expression();
  expression(const expression &pE);
  expression(const constant &pVal);
  expression(const std::string &pFldName);
  expression &operator=(const expression &pE);
  bool is_field_name() const;
  constant as_constant() const;
  std::string as_field_name() const;
  constant evaluate(scan *pS) const;
  bool applies_to(const schema &pSch) const;
  std::string to_string() const;

private:
  constant mVal;
  std::string mFldName;
};
} // namespace simpledb

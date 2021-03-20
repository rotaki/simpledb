#pragma once

#include <map>
#include <string>
#include <vector>

#include "query/constant.hpp"
#include "query/scan.hpp"

namespace simpledb {
class group_value {
  friend bool operator==(const group_value &pR, const group_value &pL);
  friend bool operator!=(const group_value &pR, const group_value &pL);

public:
  group_value(scan *s, const std::vector<std::string> &pFields);
  constant get_val(const std::string &pFldName);
  int hash_code();

private:
  std::map<std::string, constant> mVals;
};
} // namespace simpledb

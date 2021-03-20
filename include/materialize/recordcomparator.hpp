#pragma once

#include <string>
#include <vector>

#include "query/scan.hpp"

namespace simpledb {
class record_comparator {
public:
  record_comparator(const std::vector<std::string> &pFields);
  int compare(scan *s1, scan *s2);

private:
  std::vector<std::string> mFields;
};
} // namespace simpledb

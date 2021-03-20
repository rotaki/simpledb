#include "materialize/recordcomparator.hpp"

namespace simpledb {
record_comparator::record_comparator(const std::vector<std::string> &pFields)
    : mFields(pFields) {}

int record_comparator::compare(scan *s1, scan *s2) {
  for (const std::string &fldName : mFields) {
    constant val1 = s1->get_val(fldName);
    constant val2 = s2->get_val(fldName);
    if (val1 < val2) {
      return -1;
    }
    if (val2 < val1) {
      return 1;
    }
  }
  return 0;
}
} // namespace simpledb

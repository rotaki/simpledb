#include "materialize/maxfn.hpp"

namespace simpledb {
max_fn::max_fn(const std::string &pFldName) : mFldName(pFldName) {}

void max_fn::process_first(scan *pS) { mVal = pS->get_val(mFldName); }

void max_fn::process_next(scan *pS) {
  constant newVal = pS->get_val(mFldName);
  if (newVal > mVal) {
    mVal = newVal;
  }
}

std::string max_fn::field_name() { return "maxof" + mFldName; }

constant max_fn::value() { return mVal; }
} // namespace simpledb

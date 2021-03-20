#pragma once

#include "materialize/aggregationfn.hpp"

namespace simpledb {
class max_fn : public aggregation_fn {
public:
  max_fn(const std::string &pFldName);
  void process_first(scan *s) override;
  void process_next(scan *pS) override;
  std::string field_name() override;
  constant value() override;

private:
  std::string mFldName;
  constant mVal;
};
} // namespace simpledb

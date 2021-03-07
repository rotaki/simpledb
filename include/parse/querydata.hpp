#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "query/predicate.hpp"

namespace simpledb {
class query_data {
public:
  query_data(const std::vector<std::string> pFields,
             const std::set<std::string> &pTables, const predicate &pPred);

  std::vector<std::string> fields() const;

  std::set<std::string> tables() const;

  predicate pred() const;

  std::string to_string() const;

private:
  std::vector<std::string> mFields;
  std::set<std::string> mTables;
  predicate mPred;
};
} // namespace simpledb

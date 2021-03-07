#pragma once

#include <memory>
#include <string>

#include "parse/object.hpp"
#include "query/predicate.hpp"

namespace simpledb {
class delete_data : public object {
public:
  int op() override;
  delete_data(const std::string &pTblName, const predicate &pPred);
  std::string table_name() const;
  predicate pred() const;

private:
  std::string mTblName;
  predicate mPred;
};
} // namespace simpledb

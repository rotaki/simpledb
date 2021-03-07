#pragma once

#include <string>
#include <memory>

#include "query/predicate.hpp"
#include "parse/object.hpp"

namespace simpledb {
  class delete_data: public object {
  public:
    int op() override;
    delete_data(const std::string &pTblName, const predicate &pPred);
    std::string table_name() const;
    predicate pred() const;
  private:
    std::string mTblName;
    predicate mPred;
  };
}

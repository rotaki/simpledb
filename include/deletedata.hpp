#pragma once

#include <string>
#include <memory>

#include "predicate.hpp"
#include "object.hpp"

namespace smartdb {
  class delete_data: public object {
  public:
    int op() override;
    delete_data();
    delete_data(const delete_data &pDD);
    delete_data(const std::string &pTblName, const predicate &pPred);
    delete_data& operator=(const delete_data &pDD);
    std::string table_name() const;
    predicate pred() const;
  private:
    std::string mTblName;
    predicate mPred;
  };
}

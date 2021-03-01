#pragma once

#include <vector>
#include <string>
#include <set>
#include <memory>

#include "predicate.hpp"

namespace smartdb {
  class query_data {
  public:
    // query_data();

    // query_data(const query_data &pQD);
    
    query_data(const std::vector<std::string> pFields, const std::set<std::string> &pTables, const predicate &pPred);

    // query_data& operator=(const query_data &pQD);

    std::vector<std::string> fields() const;

    std::set<std::string> tables() const;

    predicate pred() const ;

    std::string to_string() const;

  private:
    std::vector<std::string> mFields;
    std::set<std::string> mTables;
    predicate mPred;
  };
}

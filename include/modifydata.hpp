#pragma once

#include <string>
#include <memory>

#include "expression.hpp"
#include "predicate.hpp"
#include "object.hpp"

namespace smartdb {
  class modify_data: public object {
  public:
    int op() override;
    // modify_data();
    // modify_data(const modify_data &pMD);
    modify_data(const std::string &pTblName, const std::string &pFldName, const expression &pNewVal, const predicate &pPred);
    // modify_data& operator=(const modify_data &pMD);
    
    std::string table_name() const;

    std::string target_field() const; 

    expression new_value() const;

    predicate pred() const;

  private:
    std::string mTblName;
    std::string mFldName;
    expression mNewVal;
    predicate mPred;
  };
}
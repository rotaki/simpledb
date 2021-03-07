#include "parse/modifydata.hpp"

namespace simpledb {
  int modify_data::op()  {
    return modify;
  }
    
  modify_data::modify_data(const std::string &pTblName, const std::string &pFldName, const expression &pNewVal, const predicate &pPred): mTblName(pTblName), mFldName(pFldName), mNewVal(pNewVal), mPred(pPred) {}

  std::string modify_data::table_name() const {
    return mTblName;
  }

  std::string modify_data::target_field() const {
    return mFldName;
  }

  expression modify_data::new_value() const {
    return mNewVal;
  }

  predicate modify_data::pred() const {
    return mPred;
  }
}

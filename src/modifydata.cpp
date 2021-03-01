#include "modifydata.hpp"

namespace smartdb {
  int modify_data::op()  {
    return modify;
  }
  
  // modify_data::modify_data() {}
  
  // modify_data::modify_data(const modify_data &pMD):
  //   mTblName(pMD.mTblName), mFldName(pMD.mFldName), mNewVal(pMD.mNewVal), mPred(pMD.mPred) {}
  
  modify_data::modify_data(const std::string &pTblName, const std::string &pFldName, const expression &pNewVal, const predicate &pPred): mTblName(pTblName), mFldName(pFldName), mNewVal(pNewVal), mPred(pPred) {}

  // modify_data& modify_data::operator=(const modify_data &pMD) {
  //   if (this != &pMD) {
  //     mTblName = pMD.mTblName;
  //     mFldName = pMD.mFldName;
  //     mNewVal = pMD.mNewVal;
  //     mPred = pMD.mPred;
  //   }
  //   return *this;
  // }

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

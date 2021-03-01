#include "deletedata.hpp"


namespace smartdb {
  int delete_data::op() {
    return remove;
  }
  
  // delete_data::delete_data() {}

  // delete_data::delete_data(const delete_data &pDD):
  //   mTblName(pDD.mTblName), mPred(pDD.mPred) {}

  delete_data::delete_data(const std::string &pTblName, const predicate &pPred):
    mTblName(pTblName), mPred(pPred) {}

  // delete_data& delete_data::operator=(const delete_data &pDD) {
  //   if (this != &pDD) {
  //     mTblName = pDD.mTblName;
  //     mPred = pDD.mPred;
  //   }
  //   return *this;
  // }

  std::string delete_data::table_name() const {
    return mTblName;
  }

  predicate delete_data::pred() const {
    return mPred;
  }
}

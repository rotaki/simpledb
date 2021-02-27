#include "insertdata.hpp"

namespace smartdb {
  insert_data::insert_data(){}
  insert_data::insert_data(const insert_data &pID):
    mTblName(pID.mTblName), mFlds(pID.mFlds), mVals(pID.mVals) {}

  insert_data::insert_data(const std::string &pTblName, std::vector<std::string> pFlds, std::vector<constant> pVals):
    mTblName(pTblName), mFlds(pFlds), mVals(pVals) {}

  int insert_data::op() {
    return insert;
  }

  insert_data& insert_data::operator=(const insert_data &pID) {
    if (this != &pID) {
      mTblName = pID.mTblName;
      mFlds = pID.mFlds;
      mVals = pID.mVals;
    }
    return *this;
  }

  std::string insert_data::table_name() const {
    return mTblName;
  }

  std::vector<std::string> insert_data::fields() const {
    return mFlds;
  }

  std::vector<constant> insert_data::vals() const {
    return mVals;
  }
}

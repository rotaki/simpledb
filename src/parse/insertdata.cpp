#include "parse/insertdata.hpp"

namespace smartdb {
  int insert_data::op() {
    return insert;
  }

  insert_data::insert_data(const std::string &pTblName, const std::vector<std::string> &pFlds, const std::vector<constant> &pVals):
    mTblName(pTblName), mFlds(pFlds), mVals(pVals) {}

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

#include "createindexdata.hpp"

namespace smartdb {
  int create_index_data::op() {
    return createindex;
  }
  // create_index_data::create_index_data() {}

  // create_index_data::create_index_data(const create_index_data &pCID):
  //   mIdxName(pCID.mIdxName), mTblName(pCID.mTblName), mFldName(pCID.mFldName) {}
  
  create_index_data::create_index_data(const std::string &pIdxName, const std::string &pTblName, const std::string &pFldName):
    mIdxName(pIdxName), mTblName(pTblName), mFldName(pFldName) {}

  // create_index_data& create_index_data::operator=(const create_index_data &pCID) {
  //   if (this != &pCID) {
  //     mIdxName = pCID.mIdxName;
  //     mTblName = pCID.mTblName;
  //     mFldName = pCID.mFldName;
  //   }
  //   return *this;
  // }

  std::string create_index_data::index_name() const {
    return mIdxName;
  }

  std::string create_index_data::table_name() const {
    return mTblName;
  }

  std::string create_index_data::field_name() const {
    return mFldName;
  }
}

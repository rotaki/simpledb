#include "createtabledata.hpp"

namespace smartdb {
  int create_table_data::op() {
    return createtable;
  }
  create_table_data::create_table_data() {}

  create_table_data::create_table_data(const create_table_data &pCTD):
    mTblName(pCTD.mTblName), mSch(pCTD.mSch) {}

  create_table_data::create_table_data(const std::string &pTblName, std::shared_ptr<schema> pSch):
    mTblName(pTblName), mSch(pSch) {}

  create_table_data& create_table_data::operator=(const create_table_data &pCTD) {
    if (this != &pCTD) {
      mTblName = pCTD.mTblName;
      mSch = pCTD.mSch;
    }
    return *this;
  }

  std::string create_table_data::table_name() const {
    return mTblName;
  }

  std::shared_ptr<schema> create_table_data::new_schema() const {
    return mSch;
  }
}

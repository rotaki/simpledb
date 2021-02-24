#include "hashindex.hpp"
#include "constant.hpp"
#include "rid.hpp"

namespace smartdb {
  int hash_index::mNumBuckets = 100;
  
  hash_index::hash_index(std::shared_ptr<transaction> pTx, const std::string &pIdxName, std::shared_ptr<layout> pLt):
    mTx(pTx), mIdxName(pIdxName), mLt(pLt) {}

  void hash_index::before_first(const constant &pSearchKey) {
    close();
    mSearchKey = pSearchKey;
    int bucket = mSearchKey.hash_code() % mNumBuckets;
    std::string tblName = mIdxName + std::to_string(bucket);
    mTS = std::shared_ptr<table_scan>(new table_scan(mTx, tblName, mLt));
  }

  bool hash_index::next() {
    while(mTS->next()) {
      if (mTS->get_val("dataval") == mSearchKey) {
        return true;
      }
    }
    return false;
  }

  rid hash_index::get_data_rid() {
    int blkNum = mTS->get_int("block");
    int id = mTS->get_int("id");
    return rid(blkNum, id);
  }

  void hash_index::insert(const constant &pVal, const rid &pRID) {
    before_first(pVal);
    mTS->insert();
    mTS->set_int("block", pRID.block_number()); 
    mTS->set_int("id", pRID.slot());
    mTS->set_val("dataval", pVal);
  }

  void hash_index::remove(const constant &pVal, const rid &pRID) {
    before_first(pVal);
    while (next()) {
      if (get_data_rid() == pRID) {
        mTS->remove();
        return;
      }
    }
  }

  void hash_index::close() {
    if (mTS) {
      mTS->close();
    }
  }

  int hash_index::search_cost(const int &pNumBlocks, const int &pRPB) {
    return pNumBlocks / mNumBuckets;
  }
}

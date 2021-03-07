#include "index/query/indexjoinscan.hpp"

namespace smartdb {
  index_join_scan::index_join_scan(const std::shared_ptr<scan> &pLhs, const std::shared_ptr<index> &pIdx, const std::string &pJoinFld, const std::shared_ptr<table_scan> &pRhs): mLhs(pLhs), mIdx(pIdx), mJoinField(pJoinFld), mRhs(pRhs)
    
  {
    before_first();
  }

  void index_join_scan::before_first() {
    mLhs->before_first();
    mLhs->next();
    reset_index();
  }

  bool index_join_scan::next() {
    while (true) {
      if (mIdx->next()) {
        mRhs->move_to_rid(mIdx->get_data_rid());
        return true;
      }
      if (!mLhs->next()) {
        return false;
      }
      reset_index();
    }
  }


  int index_join_scan::get_int(const std::string &pFldName) {
    if (mRhs->has_field(pFldName)) {
      return mRhs->get_int(pFldName);
    } else {
      return mLhs->get_int(pFldName);
    }
  }

  std::string index_join_scan::get_string(const std::string &pFldName) {
    if (mRhs->has_field(pFldName)) {
      return mRhs->get_string(pFldName);
    } else {
      return mLhs->get_string(pFldName);
    }
  }

  constant index_join_scan::get_val(const std::string &pFldName) {
    if (mRhs->has_field(pFldName)) {
      return mRhs->get_val(pFldName);
    } else {
      return mLhs->get_val(pFldName);
    }
  }

  bool index_join_scan::has_field(const std::string &pFldName) {
    return mRhs->has_field(pFldName) || mLhs->has_field(pFldName);
  }

  void index_join_scan::close() {
    mLhs->close();
    mIdx->close();
    mRhs->close();
  }

  void index_join_scan::reset_index() {
    constant searchKey = mLhs->get_val(mJoinField);
    mIdx->before_first(searchKey);
  }
}

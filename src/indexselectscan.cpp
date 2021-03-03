#include "indexselectscan.hpp"

namespace smartdb {
  index_select_scan::index_select_scan(std::shared_ptr<table_scan> pTS, std::shared_ptr<index> pIdx, const constant &pVal):
    mTS(pTS), mIdx(pIdx), mVal(pVal) {
    before_first();
  }

  void index_select_scan::before_first() {
    mIdx->before_first(mVal);
  }

  bool index_select_scan::next() {
    bool ok = mIdx->next();
    if (ok) {
      rid r = mIdx->get_data_rid();
      mTS->move_to_rid(r);
    }
    return ok;
  }

  int index_select_scan::get_int(const std::string &pFldName) {
    return mTS->get_int(pFldName);
  }

  std::string index_select_scan::get_string(const std::string &pFldName) {
    return mTS->get_string(pFldName);
  }

  constant index_select_scan::get_val(const std::string &pFldName) {
    return mTS->get_val(pFldName);
  }

  bool index_select_scan::has_field(const std::string &pFldName) {
    return mTS->has_field(pFldName);
  }

  void index_select_scan::close(){
    mIdx->close();
    mTS->close();
  }
}

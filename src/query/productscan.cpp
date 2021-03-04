#include "query/productscan.hpp"

namespace smartdb {
  product_scan::product_scan(std::shared_ptr<scan> pS1, std::shared_ptr<scan> pS2):
    mS1(pS1), mS2(pS2) {};

  void product_scan::before_first() {
    mS1->before_first();
    mS1->next();
    mS2->before_first();
  }
  
  bool product_scan::next() {
    if (mS2->next()) {
      return true;
    } else {
      mS2->before_first();
      return mS2->next() && mS1->next();
    }
  }

  int product_scan::get_int(const std::string &pFldName) {
    if (mS1->has_field(pFldName)) {
      return mS1->get_int(pFldName);
    } else {
      return mS2->get_int(pFldName);
    }
  }

  std::string product_scan::get_string(const std::string &pFldName) {
    if (mS1->has_field(pFldName)) {
      return mS1->get_string(pFldName);
    } else {
      return mS2->get_string(pFldName);
    }
  }

  constant product_scan::get_val(const std::string &pFldName) {
    if (mS1->has_field(pFldName)) {
      return mS1->get_val(pFldName);
    } else {
      return mS2->get_val(pFldName);
    }
  }

  bool product_scan::has_field(const std::string &pFldName) {
    return mS1->has_field(pFldName) && mS2->has_field(pFldName);
  }

  void product_scan::close() {
    mS1->close();
    mS2->close();
  }
}

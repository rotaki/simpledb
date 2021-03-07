#include <stdexcept>

#include "query/selectscan.hpp"

namespace simpledb {
  select_scan::select_scan(const std::shared_ptr<scan> &pS, const predicate &pP) :
    mS(pS), mP(pP)
  {}

  void select_scan::before_first() {
    mS->before_first();
  }

  bool select_scan::next() {
    while (mS->next()) {
      if (mP.is_satisfied(mS.get())) {
        return true;
      } 
    }
    return false;
  }

  int select_scan::get_int(const std::string &pFldName) {
    return mS->get_int(pFldName);
  }

  std::string select_scan::get_string(const std::string &pFldName) {
    return mS->get_string(pFldName);
  }

  constant select_scan::get_val(const std::string &pFldName) {
    return mS->get_val(pFldName);
  }

  bool select_scan::has_field(const std::string &pFldName) {
    return mS->has_field(pFldName);
  }

  void select_scan::close() {
    mS->close();
  }

  void select_scan::set_int(const std::string &pFldName, const int &pVal) {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      uS->set_int(pFldName, pVal);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void select_scan::set_string(const std::string &pFldName, const std::string &pVal) {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      uS->set_string(pFldName, pVal);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void select_scan::set_val(const std::string &pFldName, const constant &pVal) {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      uS->set_val(pFldName, pVal);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void select_scan::remove() {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      uS->remove();
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void select_scan::insert() {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      uS->insert();
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  rid select_scan::get_rid() {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      return uS->get_rid();
    } else {
      throw std::runtime_error("class cast exception");
    }
  }

  void select_scan::move_to_rid(const rid &pRID) {
    std::shared_ptr<update_scan> uS = std::dynamic_pointer_cast<update_scan>(mS);
    if (uS) {
      uS->move_to_rid(pRID);
    } else {
      throw std::runtime_error("class cast exception");
    }
  }
}

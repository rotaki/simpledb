#include <stdexcept>
#include <algorithm>

#include "projectscan.hpp"

namespace smartdb {
  project_scan::project_scan(std::shared_ptr<scan> pS, const std::vector<std::string> &pFieldList):
    mS(pS), mFieldList(pFieldList) {}

  void project_scan::before_first() {
    mS->before_first();
  }

  bool project_scan::next() {
    return mS->next();
  }

  int project_scan::get_int(const std::string &pFldName) {
    if (has_field(pFldName)) {
      return mS->get_int(pFldName);
    } else {
      throw std::runtime_error("field not found");
    }
  }

  std::string project_scan::get_string(const std::string &pFldName) {
    if (has_field(pFldName)) {
      return mS->get_string(pFldName);
    } else {
      throw std::runtime_error("field not found");
    }
  }

  constant project_scan::get_val(const std::string &pFldName) {
    if (has_field(pFldName)) {
      return mS->get_val(pFldName);
    } else {
      throw std::runtime_error("field not found");
    }
  }

  bool project_scan::has_field(const std::string &pFldName) {
    return std::find(mFieldList.begin(), mFieldList.end(), pFldName) != mFieldList.end();
  }

  void project_scan::close() {
    mS->close();
  }
}

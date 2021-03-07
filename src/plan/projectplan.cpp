#include "plan/projectplan.hpp"
#include "query/projectscan.hpp"

namespace simpledb {
project_plan::project_plan(const std::shared_ptr<plan> &pP,
                           const std::vector<std::string> &pFieldList)
    : mP(pP) {
  for (const std::string &fieldName : pFieldList) {
    mSch.add(fieldName, mP->get_schema());
  }
}

std::shared_ptr<scan> project_plan::open() {
  std::shared_ptr<scan> s = mP->open();
  return std::static_pointer_cast<scan>(
      std::make_shared<project_scan>(s, mSch.fields()));
}

int project_plan::blocks_accessed() { return mP->blocks_accessed(); }

int project_plan::records_output() { return mP->records_output(); }

int project_plan::distinct_values(const std::string &pFldName) {
  return mP->distinct_values(pFldName);
}

schema project_plan::get_schema() { return mSch; }
} // namespace simpledb

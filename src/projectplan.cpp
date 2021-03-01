#include "projectplan.hpp"
#include "projectscan.hpp"

namespace smartdb {
  project_plan::project_plan(std::shared_ptr<plan> pP, std::vector<std::string> pFieldList):
    mP(pP)
  {
    mSch = std::shared_ptr<schema>(new schema);
    for (const std::string &fieldName: pFieldList) {
      mSch->add(fieldName, mP->get_schema());
    }
  }

  std::shared_ptr<scan> project_plan::open() {
    std::shared_ptr<scan> s = mP->open();
    return std::static_pointer_cast<scan>(std::shared_ptr<project_scan>(new project_scan(s, mSch->fields())));
  }

  int project_plan::blocks_accessed() {
    return mP->blocks_accessed();
  }

  int project_plan::records_output() {
    return mP->records_output();
  }

  int project_plan::distinct_values(const std::string &pFldName) {
    return mP->distinct_values(pFldName);
  }

  std::shared_ptr<schema> project_plan::get_schema() {
    return mSch;
  }
}

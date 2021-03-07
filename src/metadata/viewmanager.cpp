#include "metadata/viewmanager.hpp"

namespace simpledb {
  view_manager::view_manager(bool pIsNew, table_manager* pTM, transaction* pTx):
    mTM(pTM)
  {
    if (pIsNew) {
      schema sch;
      sch.add_string_field("viewname", mTM->mMaxName);
      sch.add_string_field("viewdef", mMaxViewDiff);
      mTM->create_table("viewcat", sch, pTx);
    }
  }

  void view_manager::create_view(const std::string &pVName, const std::string &pVDef, transaction* pTx) {
    layout lt = mTM->get_layout("viewcat", pTx);
    table_scan tS(pTx, "viewcat", lt);
    tS.insert();
    tS.set_string("viewname", pVName); // bug
    tS.set_string("viewdef", pVDef);
    tS.close();
  }

  std::string view_manager::get_view_def(const std::string &pVName, transaction* pTx) {
    std::string result;
    layout lt = mTM->get_layout("viewcat", pTx);
    table_scan tS(pTx, "viewcat", lt);

    while (tS.next()) {
      if (tS.get_string("viewname") == pVName) {
        result = tS.get_string("viewdef");
        break;
      }
    }
    
    tS.close();
    return result;
  }
}

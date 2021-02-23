#include "viewmanager.hpp"

// todo need testing
namespace smartdb {
  view_manager::view_manager(bool pIsNew, std::shared_ptr<table_manager> pTM, std::shared_ptr<transaction> pTx):
    mTM(pTM)
  {
    if (pIsNew) {
      std::shared_ptr<schema> sch(new schema);
      sch->add_string_field("viewname", mTM->mMaxName); // todo is this possible?
      sch->add_string_field("viewdef", mMaxViewDiff);
      mTM->create_table("viewcat", sch, pTx);
    }
  }

  void view_manager::create_view(const std::string &pVName, const std::string &pVDef, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<layout> lt = mTM->get_layout("viewcat", pTx);
    std::shared_ptr<table_scan> tS(new table_scan(pTx, "viewcat", lt));
    tS->set_string("viewname", pVName);
    tS->set_string("viewdef", pVDef);
    tS->close();
  }

  std::string view_manager::get_view_def(const std::string &pVName, std::shared_ptr<transaction> pTx) {
    std::string result = nullptr;
    std::shared_ptr<layout> lt = mTM->get_layout("viewcat", pTx);
    std::shared_ptr<table_scan> tS(new table_scan(pTx, "viewcat", lt));
    while (tS->next()) {
      if (tS->get_string("viewname") == pVName) {
        result = tS->get_string("viewdef");
        break;
      }
    }
    tS->close();
    return result;
  }
}

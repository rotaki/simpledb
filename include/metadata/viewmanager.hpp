#pragma once

#include <memory>

#include "tx/transaction.hpp"
#include "metadata/tablemanager.hpp"
#include "record/tablescan.hpp"

namespace smartdb {
  class view_manager {
  public:
    view_manager(bool pIsNew, std::shared_ptr<table_manager> pTM, std::shared_ptr<transaction> pTx);
    void create_view(const std::string &pVName, const std::string &pVDef, std::shared_ptr<transaction> pTx);
    std::string get_view_def(const std::string &pVName, std::shared_ptr<transaction> pTx);
  private:
    const int mMaxViewDiff = 100; // max view def chars
    std::shared_ptr<table_manager> mTM;
  };
}

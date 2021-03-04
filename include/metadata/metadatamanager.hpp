#pragma once

#include <memory>
#include <string>
#include <map>

#include "tx/transaction.hpp"
#include "record/schema.hpp"
#include "record/layout.hpp"
#include "metadata/tablemanager.hpp"
#include "metadata/viewmanager.hpp"
#include "metadata/statmanager.hpp"
#include "metadata/indexmanager.hpp"

namespace smartdb {
  class metadata_manager {
  public:
    metadata_manager(const bool &pIsNew, std::shared_ptr<transaction> pTx);
    void create_table(const std::string &pTblName, std::shared_ptr<schema> pSch, std::shared_ptr<transaction> pTx);
    std::shared_ptr<layout> get_layout(const std::string &pTblName, std::shared_ptr<transaction> pTx);
    void create_view(const std::string &pViewName, const std::string &pViewDiff, std::shared_ptr<transaction> pTx);
    std::string get_view_def(const std::string &pViewName, std::shared_ptr<transaction> pTx);
    void create_index(const std::string &pIdxName, const std::string &pTblName, const std::string &pFldName, std::shared_ptr<transaction> pTx);
    std::map<std::string, std::shared_ptr<index_info>> get_index_info(const std::string &pTblName, std::shared_ptr<transaction> pTx);
    std::shared_ptr<stat_info> get_stat_info(const std::string &pTblName, std::shared_ptr<layout> pLt, std::shared_ptr<transaction> pTx);
  private:
    static std::shared_ptr<table_manager> mTM;
    static std::shared_ptr<view_manager> mVM;
    static std::shared_ptr<stat_manager> mSM;
    static std::shared_ptr<index_manager> mIM;
  };
}

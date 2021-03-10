#pragma once

#include <map>
#include <memory>
#include <string>

#include "metadata/indexmanager.hpp"
#include "metadata/statmanager.hpp"
#include "metadata/tablemanager.hpp"
#include "metadata/viewmanager.hpp"
#include "record/layout.hpp"
#include "record/schema.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class metadata_manager {
public:
  metadata_manager(bool pIsNew, transaction *pTx);
  void create_table(const std::string &pTblName, const schema &pSch,
                    transaction *pTx);
  layout get_layout(const std::string &pTblName, transaction *pTx);
  void create_view(const std::string &pViewName, const std::string &pViewDiff,
                   transaction *pTx);
  std::string get_view_def(const std::string &pViewName, transaction *pTx);
  void create_index(const std::string &pIdxName, const std::string &pTblName,
                    const std::string &pFldName, transaction *pTx);
  std::map<std::string, index_info> get_index_info(const std::string &pTblName,
                                                   transaction *pTx);
  stat_info get_stat_info(const std::string &pTblName, const layout &pLt,
                          transaction *pTx);

private:
  static std::unique_ptr<table_manager> mTM;
  static std::unique_ptr<view_manager> mVM;
  static std::unique_ptr<stat_manager> mSM;
  static std::unique_ptr<index_manager> mIM;
};
} // namespace simpledb

#include "metadatamanager.hpp"

namespace smartdb {
  std::shared_ptr<table_manager> metadata_manager::mTM = std::shared_ptr<table_manager>(nullptr);
  std::shared_ptr<view_manager> metadata_manager::mVM = std::shared_ptr<view_manager>(nullptr);
  std::shared_ptr<stat_manager> metadata_manager::mSM = std::shared_ptr<stat_manager>(nullptr);
  std::shared_ptr<index_manager> metadata_manager::mIM = std::shared_ptr<index_manager>(nullptr);

  metadata_manager::metadata_manager(const bool &pIsNew, std::shared_ptr<transaction> pTx) {
    mTM = std::shared_ptr<table_manager>(new table_manager(pIsNew, pTx));
    mVM = std::shared_ptr<view_manager>(new view_manager(pIsNew, mTM, pTx));
    mSM = std::shared_ptr<stat_manager>(new stat_manager(mTM, pTx));
    mIM = std::shared_ptr<index_manager>(new index_manager(pIsNew, mTM, mSM, pTx));
  }

  void metadata_manager::create_table(const std::string &pTblName, std::shared_ptr<schema> pSch, std::shared_ptr<transaction> pTx) {
    mTM->create_table(pTblName, pSch, pTx);
  }

  std::shared_ptr<layout> metadata_manager::get_layout(const std::string &pTblName, std::shared_ptr<transaction> pTx) {
    return mTM->get_layout(pTblName, pTx);
  }

  void metadata_manager::create_view(const std::string &pViewName, const std::string &pViewDef, std::shared_ptr<transaction> pTx) {
    mVM->create_view(pViewName, pViewDef, pTx);
  }

  std::string metadata_manager::get_view_def(const std::string &pViewName, std::shared_ptr<transaction> pTx) {
    return mVM->get_view_def(pViewName, pTx);
  }

  void metadata_manager::create_index(const std::string &pIdxName, const std::string &pTblName, const std::string &pFldName, std::shared_ptr<transaction> pTx) {
    mIM->create_index(pIdxName, pTblName, pFldName, pTx);
  }
  
  std::map<std::string, std::shared_ptr<index_info>> metadata_manager::get_index_info(const std::string &pTblName, std::shared_ptr<transaction> pTx) {
    return mIM->get_index_info(pTblName, pTx);
  }

  std::shared_ptr<stat_info> metadata_manager::get_stat_info(const std::string &pTblName, std::shared_ptr<layout> pLt, std::shared_ptr<transaction> pTx) {
    return mSM->get_stat_info(pTblName, pLt, pTx);
  }
}

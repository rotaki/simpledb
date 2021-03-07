#include "metadata/metadatamanager.hpp"

namespace simpledb {
std::unique_ptr<table_manager> metadata_manager::mTM;
std::unique_ptr<view_manager> metadata_manager::mVM;
std::unique_ptr<stat_manager> metadata_manager::mSM;
std::unique_ptr<index_manager> metadata_manager::mIM;

metadata_manager::metadata_manager(const bool &pIsNew, transaction *pTx) {
  mTM = std::make_unique<table_manager>(pIsNew, pTx);
  mVM = std::make_unique<view_manager>(pIsNew, mTM.get(), pTx);
  mSM = std::make_unique<stat_manager>(mTM.get(), pTx);
  mIM = std::make_unique<index_manager>(pIsNew, mTM.get(), mSM.get(), pTx);
}

void metadata_manager::create_table(const std::string &pTblName,
                                    const schema &pSch, transaction *pTx) {
  mTM->create_table(pTblName, pSch, pTx);
}

layout metadata_manager::get_layout(const std::string &pTblName,
                                    transaction *pTx) {
  return mTM->get_layout(pTblName, pTx);
}

void metadata_manager::create_view(const std::string &pViewName,
                                   const std::string &pViewDef,
                                   transaction *pTx) {
  mVM->create_view(pViewName, pViewDef, pTx);
}

std::string metadata_manager::get_view_def(const std::string &pViewName,
                                           transaction *pTx) {
  return mVM->get_view_def(pViewName, pTx);
}

void metadata_manager::create_index(const std::string &pIdxName,
                                    const std::string &pTblName,
                                    const std::string &pFldName,
                                    transaction *pTx) {
  mIM->create_index(pIdxName, pTblName, pFldName, pTx);
}

std::map<std::string, index_info>
metadata_manager::get_index_info(const std::string &pTblName,
                                 transaction *pTx) {
  return mIM->get_index_info(pTblName, pTx);
}

stat_info metadata_manager::get_stat_info(const std::string &pTblName,
                                          const layout &pLt, transaction *pTx) {
  return mSM->get_stat_info(pTblName, pLt, pTx);
}
} // namespace simpledb

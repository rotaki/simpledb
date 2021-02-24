#include "indexmanager.hpp"
#include "hashindex.hpp"
#include "schema.hpp"
#include "layout.hpp"


namespace smartdb {
  index_info::index_info(const std::string& pIdxName, const std::string &pFldName, std::shared_ptr<schema> pTblSchema, std::shared_ptr<transaction> pTx, std::shared_ptr<stat_info> pSI):
    mIdxName(pIdxName), mFldName(pFldName), mTx(pTx), mTblSchema(pTblSchema), mSI(pSI)
  {
    mIdxLayout = create_idx_layout();
  }

  std::shared_ptr<index> index_info::open() {
    std::shared_ptr<schema> sch(new schema);
    return std::shared_ptr<index>(new hash_index(mTx, mIdxName, mIdxLayout));
  }

  int index_info::blocks_accessed() {
    int rPB = mTx->block_size() / mIdxLayout->slot_size();
    int numBlocks = mSI->records_output() / rPB;
    return hash_index::search_cost(numBlocks, rPB);
  }

  int index_info::records_output() {
    return mSI->records_output() / mSI->distinct_values(mFldName);
  }

  int index_info::distinct_values(const std::string &pFldName) {
    return (mFldName == pFldName ? 1 : mSI->distinct_values(pFldName));
  }

  std::shared_ptr<layout> index_info::create_idx_layout() {
    std::shared_ptr<schema> sch(new schema);
    sch->add_int_field("block");
    sch->add_int_field("id");
    if (mTblSchema->type(mFldName) == schema::integer) {
      sch->add_int_field("dataval");
    } else {
      int fldLen = mTblSchema->length(mFldName);
      sch->add_string_field("dataval", fldLen);
    }
    return std::shared_ptr<layout>(new layout(sch));
  }

  index_manager::index_manager(const bool &pIsNew, std::shared_ptr<table_manager> pTM, std::shared_ptr<stat_manager> pSM, std::shared_ptr<transaction> pTx) {
    if (pIsNew) {
      std::shared_ptr<schema> sch(new schema);
      sch->add_string_field("indexname", pTM->mMaxName);
      sch->add_string_field("tablename", pTM->mMaxName);
      sch->add_string_field("fieldname", pTM->mMaxName);
      pTM->create_table("idxcat", sch, pTx);
    }
    mTM = pTM;
    mSM = pSM;
    mLt = mTM->get_layout("idxcat", pTx);
  }

  void index_manager::create_index(const std::string &pIdxName, const std::string &pTblName, const std::string &pFldName, std::shared_ptr<transaction> pTx) {
    std::shared_ptr<table_scan> tS(new table_scan(pTx, "idxcat", mLt));
    tS->insert();
    tS->set_string("indexname", pIdxName);
    tS->set_string("tablename", pTblName);
    tS->set_string("fieldname", pFldName);
    tS->close();
  }

  const std::map<std::string, std::shared_ptr<index_info>> index_manager::get_index_info(const std::string &pTblName, std::shared_ptr<transaction> pTx) {
    std::map<std::string, std::shared_ptr<index_info>> result;
    std::shared_ptr<table_scan> tS(new table_scan(pTx, "idxcat", mLt));
    while (tS->next()) {
      if (tS->get_string("tablename") == pTblName) {
        std::string idxName = tS->get_string("indexname");
        std::string fldName = tS->get_string("fieldname");
        std::shared_ptr<layout> tblLayout = mTM->get_layout(pTblName, pTx);
        std::shared_ptr<stat_info> tblSI = mSM->get_stat_info(pTblName, tblLayout, pTx);
        std::shared_ptr<index_info> iI(new index_info(idxName, fldName, tblLayout->get_schema(), pTx, tblSI));
        result[fldName] = iI;
      }
    }
    tS->close();
    return result;
  }
}

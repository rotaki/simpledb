#include "metadata/indexmanager.hpp"
#include "index/hash/hashindex.hpp"
#include "record/schema.hpp"
#include "record/layout.hpp"


namespace simpledb {
  index_info::index_info() {}
  index_info::index_info(const index_info &pII):
    mIdxName(pII.mIdxName), mFldName(pII.mFldName), mTx(pII.mTx), mTblSchema(pII.mTblSchema), mSI(pII.mSI), mIdxLayout(pII.mIdxLayout){}
  
  index_info::index_info(const std::string& pIdxName, const std::string &pFldName, const schema &pTblSchema, transaction* pTx, const stat_info &pSI):
    mIdxName(pIdxName), mFldName(pFldName), mTx(pTx), mTblSchema(pTblSchema), mSI(pSI)
  {
    mIdxLayout = create_idx_layout();
  }

  index_info& index_info::operator=(const index_info &pII) {
    if (this != &pII) {
      mIdxName = pII.mIdxName;
      mFldName = pII.mFldName;
      mTx = pII.mTx;
      mTblSchema = pII.mTblSchema;
      mSI = pII.mSI;
      mIdxLayout = pII.mIdxLayout;
    }
    return *this;
  }

  std::shared_ptr<index> index_info::open() const {
    return std::static_pointer_cast<index>(std::make_shared<hash_index>(mTx, mIdxName, mIdxLayout));
  }

  int index_info::blocks_accessed() {
    int rPB = mTx->block_size() / mIdxLayout.slot_size();
    int numBlocks = mSI.records_output() / rPB;
    return hash_index::search_cost(numBlocks, rPB);
  }

  int index_info::records_output() {
    return mSI.records_output() / mSI.distinct_values(mFldName);
  }

  int index_info::distinct_values(const std::string &pFldName) {
    return (mFldName == pFldName ? 1 : mSI.distinct_values(pFldName));
  }

  layout index_info::create_idx_layout() {
    schema sch;
    sch.add_int_field("block");
    sch.add_int_field("id");
    if (mTblSchema.type(mFldName) == schema::integer) {
      sch.add_int_field("dataval");
    } else {
      int fldLen = mTblSchema.length(mFldName);
      sch.add_string_field("dataval", fldLen);
    }
    return layout(sch);
  }

  index_manager::index_manager(const bool &pIsNew, table_manager* pTM, stat_manager* pSM, transaction* pTx) {
    if (pIsNew) {
      schema sch;
      sch.add_string_field("indexname", pTM->mMaxName);
      sch.add_string_field("tablename", pTM->mMaxName);
      sch.add_string_field("fieldname", pTM->mMaxName);
      pTM->create_table("idxcat", sch, pTx);
    }
    mTM = pTM;
    mSM = pSM;
    mLt = mTM->get_layout("idxcat", pTx);
  }

  void index_manager::create_index(const std::string &pIdxName, const std::string &pTblName, const std::string &pFldName, transaction* pTx) {
    table_scan tS(pTx, "idxcat", mLt);
    tS.insert();
    tS.set_string("indexname", pIdxName);
    tS.set_string("tablename", pTblName);
    tS.set_string("fieldname", pFldName);
    tS.close();
  }

  std::map<std::string, index_info> index_manager::get_index_info(const std::string &pTblName, transaction* pTx) {
    std::map<std::string, index_info> result;
    table_scan tS(pTx, "idxcat", mLt);
    while (tS.next()) {
      if (tS.get_string("tablename") == pTblName) {
        std::string idxName = tS.get_string("indexname");
        std::string fldName = tS.get_string("fieldname");
        layout tblLayout = mTM->get_layout(pTblName, pTx);
        stat_info tblSI = mSM->get_stat_info(pTblName, tblLayout, pTx);
        index_info iI(idxName, fldName, tblLayout.get_schema(), pTx, tblSI);
        result[fldName] = iI;
      }
    }
    tS.close();
    return result;
  }
}

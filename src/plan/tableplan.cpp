#include "plan/tableplan.hpp"

namespace simpledb {
table_plan::table_plan(transaction *pTx, const std::string &pTblName,
                       metadata_manager *pMM)
    : mTx(pTx), mTblName(pTblName) {
  mLt = pMM->get_layout(mTblName, mTx);
  mSI = pMM->get_stat_info(mTblName, mLt, mTx);
}

std::shared_ptr<scan> table_plan::open() {
  return std::static_pointer_cast<scan>(
      std::make_shared<table_scan>(mTx, mTblName, mLt));
}

int table_plan::blocks_accessed() { return mSI.blocks_accessed(); }

int table_plan::records_output() { return mSI.records_output(); }

int table_plan::distinct_values(const std::string &pFldName) {
  return mSI.distinct_values(pFldName);
}

schema table_plan::get_schema() { return mLt.get_schema(); }
} // namespace simpledb

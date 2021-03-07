#include "metadata/statmanager.hpp"
#include "record/tablescan.hpp"

namespace simpledb {
  stat_info::stat_info() {}

  stat_info::stat_info(const stat_info &pSI) {
    mNumBlocks = pSI.mNumBlocks;
    mNumRecs = pSI.mNumRecs;
  }
  
  stat_info::stat_info(const int &pNumBlocks, const int &pNumRecs):
    mNumBlocks(pNumBlocks), mNumRecs(pNumRecs) {}

  stat_info& stat_info::operator=(const stat_info &pSI) {
    if (this != &pSI) {
      mNumBlocks = pSI.mNumBlocks;
      mNumRecs = pSI.mNumRecs;
    }
    return *this;
  }

  int stat_info::blocks_accessed() {
    return mNumBlocks;
  }

  int stat_info::records_output() {
    return mNumRecs;
  }

  int stat_info::distinct_values(const std::string &pFldName) {
    return 1 + (mNumRecs / 3);   // this is wildly inaccurate
  }

  stat_manager::stat_manager(table_manager* pTM, transaction* pTx):
    mTM(pTM)
  {
    refresh_statistics(pTx);
  }

  stat_info stat_manager::get_stat_info(const std::string &pTblName, const layout &pLt, transaction* pTx) {
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    mNumCalls++;
    if (mNumCalls > 100) {
      refresh_statistics(pTx);
    }
    stat_info sI;
    if (mTableStats.find(pTblName) == mTableStats.end()) {
      sI = calc_table_stats(pTblName, pLt, pTx);
      mTableStats[pTblName] = sI;
    } else {
      sI = mTableStats[pTblName];
    }
    return sI;
  }

  void stat_manager::refresh_statistics(transaction* pTx) {
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    mTableStats.clear();
    mNumCalls = 0;
    layout tCatLayout = mTM->get_layout("tblcat", pTx);
    table_scan tCat(pTx, "tblcat", tCatLayout);
    while (tCat.next()) {
      std::string tblName = tCat.get_string("tblname");
      layout lt = mTM->get_layout(tblName, pTx);
      stat_info sI = calc_table_stats(tblName, lt, pTx);
      mTableStats[tblName] = sI;
    }
    tCat.close();
  }

  stat_info stat_manager::calc_table_stats(const std::string &pTblName, const layout &pLt, transaction* pTx) {
    int numRecs = 0;
    int numBlocks = 0;

    table_scan tS(pTx, pTblName, pLt);
    while (tS.next()) {
      numRecs++;
      numBlocks = tS.get_rid().block_number() + 1;
    }
    tS.close();
    return stat_info(numBlocks, numRecs);
  }
  
}

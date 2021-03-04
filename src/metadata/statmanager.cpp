#include "metadata/statmanager.hpp"
#include "record/tablescan.hpp"

namespace smartdb {
  stat_info::stat_info(const int &pNumBlocks, const int &pNumRecs):
    mNumBlocks(pNumBlocks), mNumRecs(pNumRecs) {}

  int stat_info::blocks_accessed() {
    return mNumBlocks;
  }

  int stat_info::records_output() {
    return mNumRecs;
  }

  int stat_info::distinct_values(const std::string &pFldName) {
    return 1 + (mNumRecs / 3);   // this is wildly inaccurate
  }

  stat_manager::stat_manager(std::shared_ptr<table_manager> pTM, std::shared_ptr<transaction> pTx):
    mTM(pTM)
  {
    refresh_statistics(pTx);
  }

  std::shared_ptr<stat_info> stat_manager::get_stat_info(const std::string &pTblName, std::shared_ptr<layout> pLt, std::shared_ptr<transaction> pTx) {
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    mNumCalls++;
    if (mNumCalls > 100) {
      refresh_statistics(pTx);
    }
    std::shared_ptr<stat_info> sI = mTableStats[pTblName];
    if (!sI) {
      sI = calc_table_stats(pTblName, pLt, pTx);
      mTableStats[pTblName] = sI;
    }
    return sI;
  }

  void stat_manager::refresh_statistics(std::shared_ptr<transaction> pTx) {
    std::unique_lock<std::recursive_mutex> lock(mMutex);
    mTableStats.clear();
    mNumCalls = 0;
    std::shared_ptr<layout> tCatLayout = mTM->get_layout("tblcat", pTx);
    std::shared_ptr<table_scan> tCat(new table_scan(pTx, "tblcat", tCatLayout));
    while (tCat->next()) {
      std::string tblName = tCat->get_string("tblname");
      std::shared_ptr lt = mTM->get_layout(tblName, pTx);
      std::shared_ptr<stat_info> sI = calc_table_stats(tblName, lt, pTx);
      mTableStats[tblName] = sI;
    }
    tCat->close();
  }

  std::shared_ptr<stat_info> stat_manager::calc_table_stats(const std::string &pTblName, std::shared_ptr<layout> pLt, std::shared_ptr<transaction> pTx) {
    int numRecs = 0;
    int numBlocks = 0;
    std::shared_ptr<table_scan> tS(new table_scan(pTx, pTblName, pLt));
    while (tS->next()) {
      numRecs++;
      numBlocks = tS->get_rid().block_number() + 1;
    }
    tS->close();
    return std::shared_ptr<stat_info>(new stat_info(numBlocks, numRecs));
  }
  
}

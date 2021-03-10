#pragma once

#include <map>
#include <memory>
#include <mutex>

#include "metadata/tablemanager.hpp"

namespace simpledb {
class stat_info {
public:
  stat_info();
  stat_info(const stat_info &pSI);
  stat_info(int pNumBlocks, int pNumRecs);
  stat_info &operator=(const stat_info &pSI);
  int blocks_accessed();
  int records_output();
  int distinct_values(const std::string &pFldName);

private:
  int mNumBlocks;
  int mNumRecs;
};

class stat_manager {
public:
  stat_manager(table_manager *pTM, transaction *pTx);
  stat_info get_stat_info(const std::string &pTblName, const layout &pLt,
                          transaction *pTx);

private:
  table_manager *mTM;
  std::map<std::string, stat_info> mTableStats;
  int mNumCalls;
  std::recursive_mutex mMutex;

  void refresh_statistics(transaction *pTx);
  stat_info calc_table_stats(const std::string &pTblName, const layout &pLt,
                             transaction *pTx);
};
} // namespace simpledb

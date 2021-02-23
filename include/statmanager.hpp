#pragma once

#include <memory>
#include <map>
#include <mutex>

#include "tablemanager.hpp"

namespace smartdb {
  class stat_info {
  public:
    stat_info(const int &pNumBlocks, const int &pNumRecs);
    int blocks_accessed();
    int records_output();
    int distinct_values(const std::string &pFldName);
  private:
    int mNumBlocks;
    int mNumRecs;
  };
  
  class stat_manager {
  public:
    stat_manager(std::shared_ptr<table_manager> pTM, std::shared_ptr<transaction> pTx);
    std::shared_ptr<stat_info> get_stat_info(const std::string &pTblName, std::shared_ptr<layout> pLt, std::shared_ptr<transaction> pTx);
    
  private:
    std::shared_ptr<table_manager> mTM;
    std::map<std::string, std::shared_ptr<stat_info>> mTableStats;
    int mNumCalls;
    std::recursive_mutex mMutex;
    
    void refresh_statistics(std::shared_ptr<transaction> pTx);
    std::shared_ptr<stat_info> calc_table_stats(const std::string &pTblName, std::shared_ptr<layout> pLt, std::shared_ptr<transaction> pTx);
  };
}

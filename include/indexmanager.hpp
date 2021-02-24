#pragma once

#include <memory>
#include <map>

#include "tablemanager.hpp"
#include "statmanager.hpp"
#include "index.hpp"

namespace smartdb {
  class index_manager;
  
  class index_info {
  public:
    index_info(const std::string &pIdxName, const std::string &pFldName, std::shared_ptr<schema> pTblSchema, std::shared_ptr<transaction> pTx, std::shared_ptr<stat_info> pSI);

    std::shared_ptr<index> open();
    int blocks_accessed();
    int records_output();
    int distinct_values(const std::string &pFldName);
    
  private:
    std::string mIdxName, mFldName;
    std::shared_ptr<transaction> mTx;
    std::shared_ptr<schema> mTblSchema;
    std::shared_ptr<layout> mIdxLayout;
    std::shared_ptr<stat_info> mSI;

    std::shared_ptr<layout> create_idx_layout();
  };
  
  class index_manager: public std::enable_shared_from_this<index_manager> {
    friend class index_info;
  public:
    index_manager(const bool &pIsNew, std::shared_ptr<table_manager> pTM, std::shared_ptr<stat_manager> pSM, std::shared_ptr<transaction> pTx);
    void create_index(const std::string &pIdxName, const std::string &pTblName, const std::string &pFldName, std::shared_ptr<transaction> pTx);
    const std::map<std::string, std::shared_ptr<index_info>> get_index_info(const std::string &pTblName, std::shared_ptr<transaction> pTx);

  private:
    std::shared_ptr<layout> mLt;
    std::shared_ptr<table_manager> mTM;
    std::shared_ptr<stat_manager> mSM;
  };
}

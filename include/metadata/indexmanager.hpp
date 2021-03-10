#pragma once

#include <map>
#include <memory>

#include "index/index.hpp"
#include "metadata/statmanager.hpp"
#include "metadata/tablemanager.hpp"

namespace simpledb {
class index_manager;

class index_info {
public:
  index_info();
  index_info(const index_info &pII);
  index_info(const std::string &pIdxName, const std::string &pFldName,
             const schema &pTblSchema, transaction *pTx, const stat_info &pSI);

  index_info &operator=(const index_info &pII);
  std::shared_ptr<index> open() const;
  int blocks_accessed();
  int records_output();
  int distinct_values(const std::string &pFldName);

private:
  std::string mIdxName, mFldName;
  transaction *mTx;
  schema mTblSchema;
  stat_info mSI;
  layout mIdxLayout;

  layout create_idx_layout();
};

class index_manager : public std::enable_shared_from_this<index_manager> {
  friend class index_info;

public:
  index_manager(bool pIsNew, table_manager *pTM, stat_manager *pSM,
                transaction *pTx);
  void create_index(const std::string &pIdxName, const std::string &pTblName,
                    const std::string &pFldName, transaction *pTx);
  std::map<std::string, index_info> get_index_info(const std::string &pTblName,
                                                   transaction *pTx);

private:
  table_manager *mTM;
  stat_manager *mSM;
  layout mLt;
};
} // namespace simpledb

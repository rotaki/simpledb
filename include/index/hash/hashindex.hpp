#pragma once

#include "index/index.hpp"
#include "query/constant.hpp"
#include "record/layout.hpp"
#include "record/tablescan.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class hash_index : public index {
public:
  hash_index(transaction *pTx, const std::string &pIdxName, const layout &pLt);
  void before_first(const constant &pSearckKey) override;
  bool next() override;
  rid get_data_rid() override;
  void insert(const constant &pDataVal, const rid &pDataRID) override;
  void remove(const constant &pDataVal, const rid &pDataRID) override;
  void close() override;
  static int search_cost(const int &pNumBlocks, const int &pRPB);
  static int mNumBuckets;

private:
  transaction *mTx;
  std::string mIdxName;
  layout mLt;
  constant mSearchKey;
  std::unique_ptr<table_scan> mTS;
};
} // namespace simpledb

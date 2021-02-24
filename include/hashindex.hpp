#pragma once

#include "index.hpp"
#include "transaction.hpp"
#include "layout.hpp"
#include "constant.hpp"
#include "tablescan.hpp"

namespace smartdb {
  class hash_index: public index {
  public:
    hash_index(std::shared_ptr<transaction> pTx, const std::string &pIdxName, std::shared_ptr<layout> pLt);
    void before_first(const constant &pSearckKey) override;
    bool next() override;
    rid get_data_rid() override;
    void insert(const constant &pDataVal, const rid &pDataRID) override;
    void remove(const constant &pDataVal, const rid &pDataRID) override;
    void close() override;
    static int search_cost(const int &pNumBlocks, const int &pRPB);
    static int mNumBuckets;
  private:
    std::shared_ptr<transaction> mTx;
    std::string mIdxName;
    std::shared_ptr<layout> mLt;
    constant mSearchKey;
    std::shared_ptr<table_scan> mTS;
  };
}

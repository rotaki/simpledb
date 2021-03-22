#pragma once

#include "query/scan.hpp"
#include "record/layout.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class multibuffer_product_scan : public scan {
public:
  multibuffer_product_scan(transaction *pTx,
                           const std::shared_ptr<scan> &pLhsScan,
                           const std::string &pTblName, const layout &pLt);
  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

private:
  transaction *mTx;
  std::shared_ptr<scan> mLhsScan, mRhsScan, mProdScan;
  std::string mFileName;
  layout mLt;
  int mChunkSize, mNextBlkNum, mFileSize;

  bool use_next_chunk();
};
} // namespace simpledb

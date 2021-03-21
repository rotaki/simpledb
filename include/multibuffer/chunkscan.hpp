#pragma once

#include "query/scan.hpp"
#include "record/layout.hpp"
#include "record/recordpage.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class chunk_scan : public scan {
public:
  chunk_scan(transaction *pTx, const std::string &pFileName, const layout &pLt,
             int pStartBNum, int pEndBNum);
  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

private:
  std::vector<std::unique_ptr<record_page>> mBuffs;
  transaction *mTx;
  std::string mFileName;
  layout mLt;
  int mStartBNum, mEndBNum, mCurrentBNum;
  record_page *mRP;
  int mCurrentSlot;

  void move_to_block(int pBlkNum);
};
} // namespace simpledb

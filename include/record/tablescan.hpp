#pragma once

#include <memory>
#include <string>

#include "query/updatescan.hpp"
#include "record/layout.hpp"
#include "record/recordpage.hpp"
#include "record/rid.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class table_scan : public update_scan {
public:
  table_scan(transaction *pTx, const std::string &pTableName,
             const layout &pLayout);
  ~table_scan();
  void before_first() override;
  bool next() override;
  int get_int(const std::string &pFldName) override;
  std::string get_string(const std::string &pFldName) override;
  constant get_val(const std::string &pFldName) override;
  bool has_field(const std::string &pFldName) override;
  void close() override;

  void set_int(const std::string &pFldName, int pVal) override;
  void set_string(const std::string &pFldName,
                  const std::string &pVal) override;
  void set_val(const std::string &pFldName, const constant &pVal) override;
  void insert() override;
  void remove() override;
  rid get_rid() override;
  void move_to_rid(const rid &pRID) override;

private:
  transaction *mTx;
  layout mLt;
  std::unique_ptr<record_page> mRP;
  std::string mFileName;
  int mCurrentSlot;

  void move_to_block(int pBlkNum);
  void move_to_new_block();
  bool at_last_block();
};
} // namespace simpledb

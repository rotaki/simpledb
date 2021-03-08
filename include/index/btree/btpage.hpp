#pragma once

#include <memory>

#include "file/blockid.hpp"
#include "query/constant.hpp"
#include "record/layout.hpp"
#include "record/rid.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class bt_page {
public:
  bt_page(transaction *pTx, const block_id &pCurrentBlk, const layout &pLt);
  int find_slot_before(const constant &pSearchKey);
  void close();
  bool is_full();
  block_id split(const int &pSplitPos, const int &pFlag);
  constant get_data_val(const int &pSlot);
  int get_flag();
  void set_flag(const int &pVal);
  block_id append_new(const int &pFlag);
  void format(const block_id &pBlk, const int &pFlag);

  int get_child_num(const int &pSlot);
  void insert_dir(const int &pSlot, const constant &pVal, const int &pBlkNum);
  rid get_data_rid(const int &pSlot);
  void insert_leaf(const int &pSlot, const constant &pVal, const rid &pRid);
  void remove(const int &pSlot);
  int get_num_recs();

private:
  transaction *mTx;
  block_id mCurrentBlk;
  layout mLt;

  void make_default_record(const block_id &pBlk, const int &pPos);
  int get_int(const int &pSlot, const std::string &pFldName);
  std::string get_string(const int &pSlot, const std::string &pFldName);
  constant get_val(const int &pSlot, const std::string &pFldName);
  void set_int(const int &pSlot, const std::string &pFldName, const int &pVal);
  void set_string(const int &pSlot, const std::string &pFldName,
                  const std::string &pVal);
  void set_val(const int &pSlot, const std::string &pFldName,
               const constant &pVal);
  void set_num_recs(const int &pN);
  void insert(const int &pSlot);
  void copy_record(const int &pFrom, const int &pTo);
  void transfer_recs(const int &pSlot, bt_page *pDest);
  int fld_pos(const int &pSlot, const std::string &pFldName);
  int slot_pos(const int &pSlot);
};
} // namespace simpledb

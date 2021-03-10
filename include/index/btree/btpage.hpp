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
  block_id split(int pSplitPos, int pFlag);
  constant get_data_val(int pSlot);
  int get_flag();
  void set_flag(int pVal);
  block_id append_new(int pFlag);
  void format(const block_id &pBlk, int pFlag);

  int get_child_num(int pSlot);
  void insert_dir(int pSlot, const constant &pVal, int pBlkNum);
  rid get_data_rid(int pSlot);
  void insert_leaf(int pSlot, const constant &pVal, const rid &pRid);
  void remove(int pSlot);
  int get_num_recs();

private:
  transaction *mTx;
  block_id mCurrentBlk;
  layout mLt;

  void make_default_record(const block_id &pBlk, int pPos);
  int get_int(int pSlot, const std::string &pFldName);
  std::string get_string(int pSlot, const std::string &pFldName);
  constant get_val(int pSlot, const std::string &pFldName);
  void set_int(int pSlot, const std::string &pFldName, int pVal);
  void set_string(int pSlot, const std::string &pFldName,
                  const std::string &pVal);
  void set_val(int pSlot, const std::string &pFldName, const constant &pVal);
  void set_num_recs(int pN);
  void insert(int pSlot);
  void copy_record(int pFrom, int pTo);
  void transfer_recs(int pSlot, bt_page *pDest);
  int fld_pos(int pSlot, const std::string &pFldName);
  int slot_pos(int pSlot);
};
} // namespace simpledb

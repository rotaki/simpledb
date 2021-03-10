#pragma once

#include <memory>

#include "record/layout.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class record_page {
public:
  const int empty = 0;
  const int used = 1;
  record_page(transaction *pTx, const block_id &pBlockId,
              const layout &pLayout);
  int get_int(int pSlot, const std::string &pFldName);
  std::string get_string(int pSlot, const std::string &pFldName);
  void set_int(int pSlot, const std::string &pFldname, int pVal);
  void set_string(int pSlot, const std::string &pFldName,
                  const std::string &pVal);
  void remove(int pSlot);
  void format();
  int next_after(int pSlot);
  int insert_after(int pSlot);
  block_id block() const;

private:
  transaction *mTx;
  block_id mBlockId;
  layout mLayout;

  void set_flag(int pSlot, int pFlag);
  int search_after(int pSlot, int pFlag);
  bool is_valid_slot(int pSlot) const;
  int offset(int pSlot) const;
};
} // namespace simpledb

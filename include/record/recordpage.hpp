#pragma once

#include <memory>

#include "tx/transaction.hpp"
#include "record/layout.hpp"

namespace simpledb {
  class record_page {
  public:
    const int empty = 0;
    const int used = 1;
    record_page(transaction* pTx,
                const block_id &pBlockId,
                const layout &pLayout);
    int get_int(const int &pSlot, const std::string& pFldName);
    std::string get_string(const int &pSlot, const std::string &pFldName);
    void set_int(const int &pSlot, const std::string &pFldname, const int &pVal);
    void set_string(const int &pSlot, const std::string &pFldName, const std::string &pVal);
    void remove(const int &pSlot);
    void format();
    int next_after(const int &pSlot);
    int insert_after(const int &pSlot);
    block_id block() const;
  private:
    transaction* mTx;
    block_id mBlockId;
    layout mLayout;

    void set_flag(const int &pSlot, const int &pFlag);
    int search_after(const int &pSlot, const int &pFlag);
    bool is_valid_slot(const int &pSlot) const;
    int offset(const int &pSlot) const;
  };
}

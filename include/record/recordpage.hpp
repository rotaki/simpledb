#pragma once

#include <memory>

#include "tx/transaction.hpp"
#include "record/layout.hpp"

namespace smartdb {
  class record_page {
  public:
    const int empty = 0;
    const int used = 1;
    record_page(std::shared_ptr<transaction> pTx,
                std::shared_ptr<block_id> pBlockId,
                std::shared_ptr<layout> pLayout);
    int get_int(const int &pSlot, const std::string& pFldName);
    std::string get_string(const int &pSlot, const std::string &pFldName);
    void set_int(const int &pSlot, const std::string &pFldname, const int &pVal);
    void set_string(const int &pSlot, const std::string &pFldName, const std::string &pVal);
    void remove(const int &pSlot);
    void format();
    int next_after(const int &pSlot);
    int insert_after(const int &pSlot);
    std::shared_ptr<block_id> block();
    
  private:
    std::shared_ptr<transaction> mTx;
    std::shared_ptr<block_id> mBlockId;
    std::shared_ptr<layout> mLayout;

    void set_flag(const int &pSlot, const int &pFlag);
    int search_after(const int &pSlot, const int &pFlag);
    bool is_valid_slot(const int &pSlot) const;
    int offset(const int &pSlot) const;
  };
}

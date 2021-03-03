#pragma once

#include <memory>
#include <string>

#include "blockid.hpp"
#include "transaction.hpp"
#include "layout.hpp"
#include "constant.hpp"
#include "direntry.hpp"
#include "btpage.hpp"

namespace smartdb {
  class btree_dir {
  public:
    btree_dir(std::shared_ptr<transaction>  pTx, std::shared_ptr<block_id> pBlk, std::shared_ptr<layout> pLt);
    void close();
    int search(const constant &pSearchKey);
    void make_new_root(const dir_entry &pDE);
    dir_entry insert(const dir_entry &pDE);
  private:
    std::shared_ptr<transaction> mTx;
    std::shared_ptr<layout> mLt;
    std::shared_ptr<bt_page> mContents;
    std::string mFileName;

    dir_entry insert_entry(const dir_entry &pDE);
    std::shared_ptr<block_id> find_child_block(const constant &pSearchKey);
  };
}

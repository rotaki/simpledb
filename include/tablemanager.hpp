#pragma once

#include <memory>

#include "transaction.hpp"
#include "schema.hpp"
#include "layout.hpp"

namespace smartdb {
  class table_manager {
  public:
    table_manager(bool pIsNew, std::shared_ptr<transaction> pTx) ;
    void create_table(const std::string &pTblName, std::shared_ptr<schema> pSch, std::shared_ptr<transaction> pTx);
    std::shared_ptr<layout> get_layout(const std::string &pTblName, std::shared_ptr<transaction> pTx);
    const int mMaxName = 16;
  private:
    std::shared_ptr<layout> mTCatLayout, mFCatLayout;
  };
}

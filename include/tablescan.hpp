#pragma once

#include <memory>
#include <string>

#include "transaction.hpp"
#include "layout.hpp"
#include "recordpage.hpp"
#include "rid.hpp"
#include "updatescan.hpp"

namespace smartdb {
  class tablescan: public updatescan {
  public:
    tablescan(std::shared_ptr<transaction> pTx, const std::string &pTableName, std::shared_ptr<layout> pLayout);
    ~tablescan();
    void before_first() override;
    bool next() override;
    int get_int(const std::string &pFldName) override;
    std::string get_string(const std::string &pFldName) override;
    constant get_val(const std::string &pFldName) override;
    bool has_field(const std::string &pFldName) override;
    void close() override;

    void set_int(const std::string &pFldName, const int &pVal) override;
    void set_string(const std::string &pFldName, const std::string &pVal) override;
    void set_val(const std::string &pFldName, const constant &pVal) override;
    void insert() override;
    void remove() override;
    rid get_rid() override;
    void move_to_rid(const rid &pRID) override;

  private:
    std::shared_ptr<transaction> mTx;
    std::shared_ptr<layout> mLt;
    std::shared_ptr<record_page> mRP;
    std::string mFileName;
    int mCurrentSlot;           // todo initialize??

    void move_to_block(const int &pBlkNum);
    void move_to_new_block();
    bool at_last_block();
  };
}

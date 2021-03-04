#pragma once

#include <memory>
#include <string>

#include "record/tablescan.hpp"
#include "index/index.hpp"
#include "query/constant.hpp"


namespace smartdb {
  class index_select_scan: public scan {
  public:
    index_select_scan(std::shared_ptr<table_scan> pTS, std::shared_ptr<index> pIdx, const constant &pVal);
    void before_first() override;
    bool next() override;
    int get_int( const std::string &pFldName) override;
    std::string get_string(const std::string  &pFldName) override;
    constant get_val(const std::string &pFldName) override;
    bool has_field(const std::string &pFldName) override;
    void close() override;
  private:
    std::shared_ptr<table_scan> mTS;
    std::shared_ptr<index> mIdx;
    constant mVal;
  }; 
}

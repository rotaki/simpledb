#pragma once

#include <memory>

#include "query/scan.hpp"

namespace smartdb {
  class product_scan: public scan {
  public:
    product_scan(std::shared_ptr<scan> pS1, std::shared_ptr<scan> pS2);
    
    void before_first() override;
    bool next() override;
    int get_int(const std::string &pFldName) override;
    std::string get_string(const std::string &pFldName) override;
    constant get_val(const std::string &pFldName) override;
    bool has_field(const std::string &pFldName) override;
    void close() override;
  private:
    std::shared_ptr<scan> mS1;
    std::shared_ptr<scan> mS2;
  };
}

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "scan.hpp"

namespace smartdb {
  class project_scan: public scan {
  public:
    project_scan(std::shared_ptr<scan> pS, const std::vector<std::string> &pFieldList);
    
    void before_first() override;
    bool next() override;
    int get_int(const std::string &pFldName) override;
    std::string get_string(const std::string &pFldName) override;
    constant get_val(const std::string &pFldName) override;
    bool has_field(const std::string &pFldName) override;
    void close() override;

  private:
    std::shared_ptr<scan> mS;
    std::vector<std::string > mFieldList;
  };
}



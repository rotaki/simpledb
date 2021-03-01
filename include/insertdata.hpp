#pragma once

#include <vector>
#include <string>

#include "constant.hpp"
#include "object.hpp"

namespace smartdb {
  class insert_data: public object{
  public:
    int op() override;
    // insert_data();
    // insert_data(const insert_data &pID);
    insert_data(const std::string &pTblName, std::vector<std::string> pFlds, std::vector<constant> pVals);
    // insert_data& operator=(const insert_data &pID);
    std::string table_name() const;
    std::vector<std::string> fields() const;
    std::vector<constant> vals() const;
  private:
    std::string mTblName;
    std::vector<std::string> mFlds;
    std::vector<constant> mVals;
  };
}

#pragma once

#include <string>
#include <vector>

#include "parse/object.hpp"
#include "query/constant.hpp"

namespace simpledb {
class insert_data : public object {
public:
  int op() override;
  insert_data(const std::string &pTblName,
              const std::vector<std::string> &pFlds,
              const std::vector<constant> &pVals);
  std::string table_name() const;
  std::vector<std::string> fields() const;
  std::vector<constant> vals() const;

private:
  std::string mTblName;
  std::vector<std::string> mFlds;
  std::vector<constant> mVals;
};
} // namespace simpledb

#pragma once

#include <string>

#include "parse/object.hpp"

namespace simpledb {
class create_index_data : public object {
public:
  int op() override;
  // create_index_data();
  // create_index_data(const create_index_data &pCID);
  create_index_data(const std::string &pIdxName, const std::string &pTblName,
                    const std::string &pFldName);
  // create_index_data& operator=(const create_index_data &pCID);
  std::string index_name() const;
  std::string table_name() const;
  std::string field_name() const;

private:
  std::string mIdxName;
  std::string mTblName;
  std::string mFldName;
};
} // namespace simpledb

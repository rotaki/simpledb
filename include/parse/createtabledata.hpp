#pragma once

#include <memory>
#include <string>

#include "parse/object.hpp"
#include "record/schema.hpp"

namespace simpledb {
class create_table_data : public object {
public:
  int op() override;
  create_table_data(const std::string &pTblName, const schema &pSch);
  std::string table_name() const;
  schema new_schema() const;

private:
  std::string mTblName;
  schema mSch;
};
} // namespace simpledb

#pragma once

#include <string>
#include <memory>

#include "record/schema.hpp"
#include "parse/object.hpp"

namespace smartdb {
  class create_table_data: public object {
  public:
    int op() override;
    create_table_data(const std::string &pTblName, const schema &pSch);
    std::string table_name() const;
    schema new_schema() const;
  private:
    std::string mTblName;
    schema mSch;
  };
}

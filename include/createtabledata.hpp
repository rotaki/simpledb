#pragma once

#include <string>
#include <memory>

#include "schema.hpp"
#include "object.hpp"

namespace smartdb {
  class create_table_data: public object {
  public:
    int op() override;
    // create_table_data();
    // create_table_data(const create_table_data &pCTD);
    create_table_data(const std::string &pTblName, std::shared_ptr<schema> pSch);
    // create_table_data& operator=(const create_table_data &pCTD);
    std::string table_name() const;
    std::shared_ptr<schema> new_schema() const;
  private:
    std::string mTblName;
    std::shared_ptr<schema> mSch;
  };
}

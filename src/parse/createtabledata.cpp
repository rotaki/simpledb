#include "parse/createtabledata.hpp"

namespace simpledb {
int create_table_data::op() { return createtable; }

create_table_data::create_table_data(const std::string &pTblName,
                                     const schema &pSch)
    : mTblName(pTblName), mSch(pSch) {}

std::string create_table_data::table_name() const { return mTblName; }

schema create_table_data::new_schema() const { return mSch; }
} // namespace simpledb

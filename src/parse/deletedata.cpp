#include "parse/deletedata.hpp"

namespace simpledb {
int delete_data::op() { return remove; }

delete_data::delete_data(const std::string &pTblName, const predicate &pPred)
    : mTblName(pTblName), mPred(pPred) {}

std::string delete_data::table_name() const { return mTblName; }

predicate delete_data::pred() const { return mPred; }
} // namespace simpledb

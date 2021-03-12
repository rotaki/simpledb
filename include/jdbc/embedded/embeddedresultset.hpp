#pragma once

#include "jdbc/resultsetadapter.hpp"
#include "plan/plan.hpp"
#include "query/scan.hpp"
#include "record/schema.hpp"

namespace simpledb {
class EmbeddedConnection;
class EmbeddedResultSet : public ResultSetAdapter {
public:
  EmbeddedResultSet(plan &pPlan, EmbeddedConnection &pConn);
  bool next();
  int32_t getInt(const sql::SQLString &pColumnLabel) const;
  sql::SQLString getString(const sql::SQLString &pColumnLabel) const;
  sql::ResultSetMetaData *getMetaData() const;
  void close();

private:
  EmbeddedConnection &mConn;
  std::shared_ptr<scan> mS;
  schema mSch;
};
} // namespace simpledb

#pragma once

#include "jdbc/embedded/embeddedresultset.hpp"
#include "jdbc/embedded/embeddedstatement.hpp"
#include "jdbc/statementadapter.hpp"
#include "plan/planner.hpp"

namespace simpledb {
class EmbeddedConnection;

class EmbeddedStatement : public StatementAdapter {
public:
  EmbeddedStatement(EmbeddedConnection &pConn, planner &pPlanner);
  EmbeddedResultSet *executeQuery(const sql::SQLString &pSQL);

  int executeUpdate(const sql::SQLString &pSQL);

  void close();

private:
  EmbeddedConnection &mConn;
  planner &mPlanner;
};

} // namespace simpledb

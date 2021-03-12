#include "jdbc/embedded/embeddedstatement.hpp"
#include "jdbc/embedded/embeddedconnection.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
EmbeddedStatement::EmbeddedStatement(EmbeddedConnection &pConn,
                                     planner &pPlanner)
    : mConn(pConn), mPlanner(pPlanner) {}

EmbeddedResultSet *EmbeddedStatement::executeQuery(const sql::SQLString &pSQL) {
  try {
    transaction &tx = mConn.getTransaction();
    auto pln = mPlanner.create_query_plan(pSQL.asStdString(), &tx);
    return new EmbeddedResultSet(*pln, mConn);
  } catch (const std::exception &e) {
    mConn.rollback();
    throw sql::SQLException(e.what());
  }
}

int EmbeddedStatement::executeUpdate(const sql::SQLString &pSQL) {
  try {
    transaction &tx = mConn.getTransaction();
    int result = mPlanner.execute_update(pSQL.asStdString(), &tx);
    mConn.commit();
    return result;
  } catch (const std::exception &e) {
    mConn.rollback();
    throw sql::SQLException(e.what());
  }
}

void EmbeddedStatement::close() { return; }
} // namespace simpledb

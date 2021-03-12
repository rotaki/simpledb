#pragma once

#include <memory>

#include "jdbc/connectionadapter.hpp"
#include "jdbc/embedded/embeddedstatement.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"

namespace simpledb {
class EmbeddedConnection : public ConnectionAdapter {
public:
  EmbeddedConnection(std::unique_ptr<simpledb> pDb);
  EmbeddedStatement *createStatement() override;
  void close() override;
  void commit() override;
  void rollback() override;
  transaction &getTransaction();

private:
  std::unique_ptr<simpledb> mDb;
  std::unique_ptr<transaction> mCurrentTx;
  planner &mPlanner;
};
} // namespace simpledb

#include "jdbc/embedded/embeddedconnection.hpp"

namespace simpledb {
EmbeddedConnection::EmbeddedConnection(std::unique_ptr<simpledb> pDb)
    : mDb(std::move(pDb)), mCurrentTx(mDb->new_tx()), mPlanner(mDb->plnr()) {}

EmbeddedStatement *EmbeddedConnection::createStatement() {
  return new EmbeddedStatement(*this, mPlanner);
}

void EmbeddedConnection::close() { mCurrentTx->commit(); }

void EmbeddedConnection::commit() {
  mCurrentTx->commit();
  mCurrentTx = mDb->new_tx();
}

void EmbeddedConnection::rollback() {
  mCurrentTx->rollback();
  mCurrentTx = mDb->new_tx();
}

transaction &EmbeddedConnection::getTransaction() { return *mCurrentTx; }
} // namespace simpledb

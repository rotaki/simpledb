#include "cppconn/exception.h"

#include "jdbc/statementadapter.hpp"

namespace simpledb {
sql::Connection *StatementAdapter::getConnection() {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::cancel() {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::clearWarnings() {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::close() {
  throw sql::SQLException("operation not implemented");
}

bool StatementAdapter::execute(const sql::SQLString &sql) {
  throw sql::SQLException("operation not implemented");
}

sql::ResultSet *StatementAdapter::executeQuery(const sql::SQLString &sql) {
  throw sql::SQLException("operation not implemented");
}

int StatementAdapter::executeUpdate(const sql::SQLString &sql) {
  throw sql::SQLException("operation not implemented");
}

size_t StatementAdapter::getFetchSize() {
  throw sql::SQLException("operation not implemented");
}

unsigned int StatementAdapter::getMaxFieldSize() {
  throw sql::SQLException("operation not implemented");
}

uint64_t StatementAdapter::getMaxRows() {
  throw sql::SQLException("operation not implemented");
}

bool StatementAdapter::getMoreResults() {
  throw sql::SQLException("operation not implemented");
}

unsigned int StatementAdapter::getQueryTimeout() {
  throw sql::SQLException("operation not implemented");
}

sql::ResultSet *StatementAdapter::getResultSet() {
  throw sql::SQLException("operation not implemented");
}

sql::ResultSet::enum_type StatementAdapter::getResultSetType() {
  throw sql::SQLException("operation not implemented");
}

uint64_t StatementAdapter::getUpdateCount() {
  throw sql::SQLException("operation not implemented");
}

const sql::SQLWarning *StatementAdapter::getWarnings() {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::setCursorName(const sql::SQLString &name) {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::setEscapeProcessing(bool enable) {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::setFetchSize(size_t rows) {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::setMaxFieldSize(unsigned int max) {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::setMaxRows(unsigned int max) {
  throw sql::SQLException("operation not implemented");
}

void StatementAdapter::setQueryTimeout(unsigned int seconds) {
  throw sql::SQLException("operation not implemented");
}

sql::Statement *
StatementAdapter::setResultSetType(sql::ResultSet::enum_type type) {
  throw sql::SQLException("operation not implemented");
}

} // namespace simpledb

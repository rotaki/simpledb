#include "cppconn/exception.h"

#include "jdbc/resultsetadapter.hpp"

namespace simpledb {
bool ResultSetAdapter::absolute(int row) {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::afterLast() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::beforeFirst() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::cancelRowUpdates() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::clearWarnings() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::close() {
  throw sql::SQLException("operation not implemented");
};

uint32_t ResultSetAdapter::findColumn(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::first() {
  throw sql::SQLException("operation not implemented");
};

std::istream *ResultSetAdapter::getBlob(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
std::istream *
ResultSetAdapter::getBlob(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::getBoolean(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
bool ResultSetAdapter::getBoolean(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

int ResultSetAdapter::getConcurrency() {
  throw sql::SQLException("operation not implemented");
};
sql::SQLString ResultSetAdapter::getCursorName() {
  throw sql::SQLException("operation not implemented");
};

long double ResultSetAdapter::getDouble(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
long double
ResultSetAdapter::getDouble(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

int ResultSetAdapter::getFetchDirection() {
  throw sql::SQLException("operation not implemented");
};
size_t ResultSetAdapter::getFetchSize() {
  throw sql::SQLException("operation not implemented");
};
int ResultSetAdapter::getHoldability() {
  throw sql::SQLException("operation not implemented");
};

int32_t ResultSetAdapter::getInt(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
int32_t ResultSetAdapter::getInt(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

uint32_t ResultSetAdapter::getUInt(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
uint32_t ResultSetAdapter::getUInt(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

int64_t ResultSetAdapter::getInt64(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
int64_t ResultSetAdapter::getInt64(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

uint64_t ResultSetAdapter::getUInt64(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
uint64_t ResultSetAdapter::getUInt64(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

sql::ResultSetMetaData *ResultSetAdapter::getMetaData() const {
  throw sql::SQLException("operation not implemented");
};

size_t ResultSetAdapter::getRow() const {
  throw sql::SQLException("operation not implemented");
};

sql::RowID *ResultSetAdapter::getRowId(uint32_t columnIndex) {
  throw sql::SQLException("operation not implemented");
};
sql::RowID *ResultSetAdapter::getRowId(const sql::SQLString &columnLabel) {
  throw sql::SQLException("operation not implemented");
};

const sql::Statement *ResultSetAdapter::getStatement() const {
  throw sql::SQLException("operation not implemented");
};

sql::SQLString ResultSetAdapter::getString(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
sql::SQLString
ResultSetAdapter::getString(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

ResultSetAdapter::enum_type ResultSetAdapter::getType() const {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::getWarnings() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::insertRow() {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::isAfterLast() const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::isBeforeFirst() const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::isClosed() const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::isFirst() const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::isLast() const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::isNull(uint32_t columnIndex) const {
  throw sql::SQLException("operation not implemented");
};
bool ResultSetAdapter::isNull(const sql::SQLString &columnLabel) const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::last() {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::next() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::moveToCurrentRow() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::moveToInsertRow() {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::previous() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::refreshRow() {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::relative(int rows) {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::rowDeleted() {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::rowInserted() {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::rowUpdated() {
  throw sql::SQLException("operation not implemented");
};

void ResultSetAdapter::setFetchSize(size_t rows) {
  throw sql::SQLException("operation not implemented");
};

size_t ResultSetAdapter::rowsCount() const {
  throw sql::SQLException("operation not implemented");
};

bool ResultSetAdapter::wasNull() const {
  throw sql::SQLException("operation not implemented");
};

} // namespace simpledb

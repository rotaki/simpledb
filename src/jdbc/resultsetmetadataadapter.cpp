#include "cppconn/exception.h"

#include "jdbc/resultsetmetadataadapter.hpp"

namespace simpledb {
sql::SQLString ResultSetMetaDataAdapter::getCatalogName(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

unsigned int ResultSetMetaDataAdapter::getColumnCount() {
  throw sql::SQLException("operation not implemented");
}

unsigned int
ResultSetMetaDataAdapter::getColumnDisplaySize(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString ResultSetMetaDataAdapter::getColumnLabel(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString ResultSetMetaDataAdapter::getColumnName(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

int ResultSetMetaDataAdapter::getColumnType(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString
ResultSetMetaDataAdapter::getColumnTypeName(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString
ResultSetMetaDataAdapter::getColumnCharset(unsigned int columnIndex) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString
ResultSetMetaDataAdapter::getColumnCollation(unsigned int columnIndex) {
  throw sql::SQLException("operation not implemented");
}

unsigned int ResultSetMetaDataAdapter::getPrecision(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

unsigned int ResultSetMetaDataAdapter::getScale(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString ResultSetMetaDataAdapter::getSchemaName(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

sql::SQLString ResultSetMetaDataAdapter::getTableName(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isAutoIncrement(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isCaseSensitive(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isCurrency(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isDefinitelyWritable(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

int ResultSetMetaDataAdapter::isNullable(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isNumeric(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isReadOnly(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isSearchable(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isSigned(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isWritable(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

bool ResultSetMetaDataAdapter::isZerofill(unsigned int column) {
  throw sql::SQLException("operation not implemented");
}

} // namespace simpledb

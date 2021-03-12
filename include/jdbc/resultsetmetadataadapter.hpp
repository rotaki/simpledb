#pragma once
#include <cppconn/resultset_metadata.h>

namespace simpledb {
class ResultSetMetaDataAdapter : public sql::ResultSetMetaData {
  virtual sql::SQLString getCatalogName(unsigned int column) override;

  virtual unsigned int getColumnCount() override;

  virtual unsigned int getColumnDisplaySize(unsigned int column) override;

  virtual sql::SQLString getColumnLabel(unsigned int column) override;

  virtual sql::SQLString getColumnName(unsigned int column) override;

  virtual int getColumnType(unsigned int column) override;

  virtual sql::SQLString getColumnTypeName(unsigned int column) override;

  virtual sql::SQLString getColumnCharset(unsigned int columnIndex) override;

  virtual sql::SQLString getColumnCollation(unsigned int columnIndex) override;

  virtual unsigned int getPrecision(unsigned int column) override;

  virtual unsigned int getScale(unsigned int column) override;

  virtual sql::SQLString getSchemaName(unsigned int column) override;

  virtual sql::SQLString getTableName(unsigned int column) override;

  virtual bool isAutoIncrement(unsigned int column) override;

  virtual bool isCaseSensitive(unsigned int column) override;

  virtual bool isCurrency(unsigned int column) override;

  virtual bool isDefinitelyWritable(unsigned int column) override;

  virtual int isNullable(unsigned int column) override;

  virtual bool isNumeric(unsigned int column) override;

  virtual bool isReadOnly(unsigned int column) override;

  virtual bool isSearchable(unsigned int column) override;

  virtual bool isSigned(unsigned int column) override;

  virtual bool isWritable(unsigned int column) override;

  virtual bool isZerofill(unsigned int column) override;
};
} // namespace simpledb

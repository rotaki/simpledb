#pragma once
#include "mysql-connector-c++/cppconn/resultset.h"

namespace simpledb {
class ResultSetAdapter : public sql::ResultSet {
public:
  virtual bool absolute(int row) override;

  virtual void afterLast() override;

  virtual void beforeFirst() override;

  virtual void cancelRowUpdates() override;

  virtual void clearWarnings() override;

  virtual void close() override;

  virtual uint32_t findColumn(const sql::SQLString &columnLabel) const override;

  virtual bool first() override;

  virtual std::istream *getBlob(uint32_t columnIndex) const override;
  virtual std::istream *
  getBlob(const sql::SQLString &columnLabel) const override;

  virtual bool getBoolean(uint32_t columnIndex) const override;
  virtual bool getBoolean(const sql::SQLString &columnLabel) const override;

  virtual int getConcurrency() override;
  virtual sql::SQLString getCursorName() override;

  virtual long double getDouble(uint32_t columnIndex) const override;
  virtual long double
  getDouble(const sql::SQLString &columnLabel) const override;

  virtual int getFetchDirection() override;
  virtual size_t getFetchSize() override;
  virtual int getHoldability() override;

  virtual int32_t getInt(uint32_t columnIndex) const override;
  virtual int32_t getInt(const sql::SQLString &columnLabel) const override;

  virtual uint32_t getUInt(uint32_t columnIndex) const override;
  virtual uint32_t getUInt(const sql::SQLString &columnLabel) const override;

  virtual int64_t getInt64(uint32_t columnIndex) const override;
  virtual int64_t getInt64(const sql::SQLString &columnLabel) const override;

  virtual uint64_t getUInt64(uint32_t columnIndex) const override;
  virtual uint64_t getUInt64(const sql::SQLString &columnLabel) const override;

  virtual sql::ResultSetMetaData *getMetaData() const override;

  virtual size_t getRow() const override;

  virtual sql::RowID *getRowId(uint32_t columnIndex) override;
  virtual sql::RowID *getRowId(const sql::SQLString &columnLabel) override;

  virtual const sql::Statement *getStatement() const override;

  virtual sql::SQLString getString(uint32_t columnIndex) const override;
  virtual sql::SQLString
  getString(const sql::SQLString &columnLabel) const override;

  virtual ResultSetAdapter::enum_type getType() const override;

  virtual void getWarnings() override;

  virtual void insertRow() override;

  virtual bool isAfterLast() const override;

  virtual bool isBeforeFirst() const override;

  virtual bool isClosed() const override;

  virtual bool isFirst() const override;

  virtual bool isLast() const override;

  virtual bool isNull(uint32_t columnIndex) const override;
  virtual bool isNull(const sql::SQLString &columnLabel) const override;

  virtual bool last() override;

  virtual bool next() override;

  virtual void moveToCurrentRow() override;

  virtual void moveToInsertRow() override;

  virtual bool previous() override;

  virtual void refreshRow() override;

  virtual bool relative(int rows) override;

  virtual bool rowDeleted() override;

  virtual bool rowInserted() override;

  virtual bool rowUpdated() override;

  virtual void setFetchSize(size_t rows) override;

  virtual size_t rowsCount() const override;

  virtual bool wasNull() const override;
};
} // namespace simpledb

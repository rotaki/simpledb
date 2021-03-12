#pragma once
#include <cppconn/statement.h>

namespace simpledb {
class StatementAdapter : public sql::Statement {
public:
  virtual sql::Connection *getConnection() override;

  virtual void cancel() override;

  virtual void clearWarnings() override;

  virtual void close() override;

  virtual bool execute(const sql::SQLString &sql) override;

  virtual sql::ResultSet *executeQuery(const sql::SQLString &sql) override;

  virtual int executeUpdate(const sql::SQLString &sql) override;

  virtual size_t getFetchSize() override;

  virtual unsigned int getMaxFieldSize() override;

  virtual uint64_t getMaxRows() override;

  virtual bool getMoreResults() override;

  virtual unsigned int getQueryTimeout() override;

  virtual sql::ResultSet *getResultSet() override;

  virtual sql::ResultSet::enum_type getResultSetType() override;

  virtual uint64_t getUpdateCount() override;

  virtual const sql::SQLWarning *getWarnings() override;

  virtual void setCursorName(const sql::SQLString &name) override;

  virtual void setEscapeProcessing(bool enable) override;

  virtual void setFetchSize(size_t rows) override;

  virtual void setMaxFieldSize(unsigned int max) override;

  virtual void setMaxRows(unsigned int max) override;

  virtual void setQueryTimeout(unsigned int seconds) override;

  virtual sql::Statement *
  setResultSetType(sql::ResultSet::enum_type type) override;
};
} // namespace simpledb

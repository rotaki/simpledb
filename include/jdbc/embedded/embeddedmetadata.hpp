#pragma once

#include "jdbc/resultsetmetadataadapter.hpp"
#include "record/schema.hpp"

namespace simpledb {
class EmbeddedMetaData : public ResultSetMetaDataAdapter {
public:
  EmbeddedMetaData(const schema &pSch);
  unsigned int getColumnCount() override;
  sql::SQLString getColumnName(unsigned int pColumn) override;
  int getColumnType(unsigned int pColumn) override;
  unsigned int getColumnDisplaySize(unsigned int pColumn) override;

private:
  schema mSch;
};
} // namespace simpledb

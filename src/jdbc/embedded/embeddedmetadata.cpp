#include <algorithm>

#include "jdbc/embedded/embeddedmetadata.hpp"

namespace simpledb {
EmbeddedMetaData::EmbeddedMetaData(const schema &pSch) : mSch(pSch) {}

unsigned int EmbeddedMetaData::getColumnCount() { return mSch.fields().size(); }

sql::SQLString EmbeddedMetaData::getColumnName(unsigned int pColumn) {
  return sql::SQLString(mSch.fields()[pColumn - 1]);
}

int EmbeddedMetaData::getColumnType(unsigned int pColumn) {
  std::string fldName = getColumnName(pColumn).asStdString();
  return mSch.type(fldName);
}

unsigned int EmbeddedMetaData::getColumnDisplaySize(unsigned int pColumn) {
  std::string fldName = getColumnName(pColumn).asStdString();
  int fldType = mSch.type(fldName);
  int fldLength = (fldType == schema::integer) ? 6 : mSch.length(fldName);
  return std::max(static_cast<int>(fldName.size()), fldLength) + 1;
}
} // namespace simpledb

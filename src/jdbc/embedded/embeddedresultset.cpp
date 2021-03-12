#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

#include "jdbc/embedded/embeddedconnection.hpp"
#include "jdbc/embedded/embeddedmetadata.hpp"
#include "jdbc/embedded/embeddedresultset.hpp"

namespace simpledb {
EmbeddedResultSet::EmbeddedResultSet(plan &pPlan, EmbeddedConnection &pConn)
    : mConn(pConn), mS(pPlan.open()), mSch(pPlan.get_schema()) {}

bool EmbeddedResultSet::next() {
  try {
    return mS->next();
  } catch (const std::exception &e) {
    mConn.rollback();
    throw sql::SQLException(e.what());
  }
}

int32_t EmbeddedResultSet::getInt(const sql::SQLString &pColumnLabel) const {
  try {
    std::string fldName = pColumnLabel.asStdString();
    std::transform(fldName.begin(), fldName.end(), fldName.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return mS->get_int(fldName);
  } catch (const std::exception &e) {
    mConn.rollback();
    throw sql::SQLException(e.what());
  }
}

sql::SQLString
EmbeddedResultSet::getString(const sql::SQLString &pColumnLabel) const {
  try {
    std::string fldName = pColumnLabel.asStdString();
    std::transform(fldName.begin(), fldName.end(), fldName.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return mS->get_string(fldName);
  } catch (const std::exception &e) {
    mConn.rollback();
    throw sql::SQLException(e.what());
  }
}

sql::ResultSetMetaData *EmbeddedResultSet::getMetaData() const {
  return new EmbeddedMetaData(mSch);
}

void EmbeddedResultSet::close() {
  mS->close();
  mConn.commit();
}
} // namespace simpledb

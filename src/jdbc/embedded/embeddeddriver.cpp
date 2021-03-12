#include "jdbc/embedded/embeddeddriver.hpp"

namespace simpledb {
EmbeddedConnection *EmbeddedDriver::connect(const sql::SQLString &pHostName,
                                            const sql::SQLString &pUsername,
                                            const sql::SQLString &pPassword) {
  sql::SQLString url = pUsername + ":" + pPassword + "@" + pHostName;
  auto db = std::make_unique<simpledb>(url.asStdString());
  return new EmbeddedConnection(std::move(db));
}
} // namespace simpledb

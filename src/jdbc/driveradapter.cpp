#include "mysql-connector-c++/cppconn/exception.h"

#include "jdbc/driveradapter.hpp"

namespace simpledb {
sql::Connection *DriverAdapter::connect(const sql::SQLString &hostName,
                                        const sql::SQLString &userName,
                                        const sql::SQLString &password) {
  throw sql::SQLException("operation not implemented");
}
sql::Connection *DriverAdapter::connect(sql::ConnectOptionsMap &options) {
  throw sql::SQLException("operation not implemented");
}
int DriverAdapter::getMajorVersion() { return 0; }
int DriverAdapter::getMinorVersion() { return 0; }
int DriverAdapter::getPatchVersion() { return 0; }
const sql::SQLString &DriverAdapter::getName() {
  throw sql::SQLException("operation not implemented");
}
void DriverAdapter::threadInit() {
  throw sql::SQLException("operation not implemented");
}
void DriverAdapter::threadEnd() {
  throw sql::SQLException("operation not implemented");
}
} // namespace simpledb

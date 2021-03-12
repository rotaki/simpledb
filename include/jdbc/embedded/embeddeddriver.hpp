#pragma once

#include "jdbc/driveradapter.hpp"
#include "jdbc/embedded/embeddedconnection.hpp"

namespace simpledb {
class EmbeddedDriver : public DriverAdapter {
public:
  EmbeddedConnection *connect(const sql::SQLString &hostName,
                              const sql::SQLString &userName,
                              const sql::SQLString &password);
};
} // namespace simpledb

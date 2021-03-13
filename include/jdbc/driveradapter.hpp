#pragma once

#include "cppconn/driver.h"

namespace simpledb {
class DriverAdapter : public sql::Driver {
public:
  virtual sql::Connection *connect(const sql::SQLString &hostName,
                                   const sql::SQLString &userName,
                                   const sql::SQLString &password) override;
  virtual sql::Connection *connect(sql::ConnectOptionsMap &options) override;
  virtual int getMajorVersion() override;
  virtual int getMinorVersion() override;
  virtual int getPatchVersion() override;
  virtual const sql::SQLString &getName() override;
  virtual void threadInit() override;
  virtual void threadEnd() override;
};
} // namespace simpledb

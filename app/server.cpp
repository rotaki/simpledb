#include "jdbc/embedded/embeddeddriver.hpp"
#include "jdbc/embedded/embeddedmetadata.hpp"
#include "utils/utils.hpp"
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cstdio>
#include <memory>
#include <vector>

namespace simpledb {
void do_query(sql::Statement &stmt, const std::string &cmd) {
  try {
    std::unique_ptr<sql::ResultSet> rS(stmt.executeQuery(cmd));
    sql::ResultSetMetaData *md(
        rS->getMetaData()); // this will be deleted automatiaclly
    int numCols = md->getColumnCount();
    int totalWidth = 0;

    for (int i = 1; i <= numCols; i++) {
      std::string fldName = md->getColumnName(i);
      int width = md->getColumnDisplaySize(i);
      totalWidth += width;
      std::string fmt = "%" + std::to_string(width) + "s";
      printf(fmt.c_str(), fldName.c_str());
    }
    std::cout << std::endl;
    for (int i = 0; i < totalWidth; i++) {
      std::cout << "-";
    }
    std::cout << std::endl;
    while (rS->next()) {
      for (int i = 1; i <= numCols; i++) {
        std::string fldName = md->getColumnName(i);
        int fldType = md->getColumnType(i);
        std::string fmt = "%" + std::to_string(md->getColumnDisplaySize(i));
        if (fldType == schema::integer) {
          int iVal = rS->getInt(fldName);
          printf((fmt + "d").c_str(), iVal);
        } else {
          std::string sVal = rS->getString(fldName);
          printf((fmt + "s").c_str(), sVal.c_str());
        }
      }
      std::cout << std::endl;
    }
    rS->close();
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__ << " on line "
              << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}

void do_update(sql::Statement &stmt, const std::string cmd) {
  try {
    int howMany = stmt.executeUpdate(cmd);
    std::cout << howMany << " records processed" << std::endl;
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__ << " on line "
              << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}

void start_server() {
  std::cout << "Connect> ";
  std::string s;
  getline(std::cin, s);
  std::unique_ptr<DriverAdapter> d = std::make_unique<EmbeddedDriver>();
  try {
    int pos1 = s.find(":");
    int pos2 = s.find("@");
    std::string username = s.substr(0, pos1);
    std::string password = s.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string host = s.substr(pos2 + 1);
    std::unique_ptr<sql::Connection> conn(d->connect(host, username, password));
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());

    std::cout << "\nSQL> ";
    std::string cmd;
    while (getline(std::cin, cmd)) {
      trim(cmd);
      if (static_cast<int>(cmd.rfind("exit", 0)) == 0) {
        break;
      } else if (static_cast<int>(cmd.rfind("select", 0)) == 0) {
        do_query(*stmt, cmd);
      } else if (static_cast<int>(cmd.rfind("insert", 0)) == 0 ||
                 static_cast<int>(cmd.rfind("delete", 0)) == 0 ||
                 static_cast<int>(cmd.rfind("update", 0)) == 0 ||
                 static_cast<int>(cmd.rfind("create", 0)) == 0) {
        do_update(*stmt, cmd);
      } else {
        std::cout << "Unknown operation";
      }
      std::cout << "\nSQL> ";
    }
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__ << " on line "
              << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}

} // namespace simpledb

int main() { simpledb::start_server(); }

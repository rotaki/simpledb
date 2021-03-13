#include <memory>
#include <vector>

#include "jdbc/embedded/embeddeddriver.hpp"
#include "mysql-connector-c++/cppconn/connection.h"
#include "mysql-connector-c++/cppconn/exception.h"

namespace simpledb {
std::string host = "localhost";
std::string username = "root";
std::string password = "password";

void create_student_database() {
  std::unique_ptr<DriverAdapter> d = std::make_unique<EmbeddedDriver>();

  try {
    std::unique_ptr<sql::Connection> conn(d->connect(host, username, password));
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());

    std::string s = "create table STUDENT(SId int, SName varchar(10), MajorId "
                    "int, GradYear int)";

    stmt->executeUpdate(s);
    std::cout << "Table STUDENT created" << std::endl;

    s = "insert into STUDENT(SId, SName, MajorId, GradYear) values ";
    std::vector<std::string> studVals = {
        "(1, 'joe', 10, 2021)", "(2, 'amy', 20, 2020)", "(3, 'max', 10, 2022)",
        "(4, 'sue', 20, 2022)", "(5, 'bob', 30, 2020)", "(6, 'kim', 20, 2020)",
        "(7, 'art', 30, 2021)", "(8, 'pat', 20, 2019)", "(9, 'lee', 10, 2021)"};
    for (int i = 0; i < static_cast<int>(studVals.size()); i++) {
      stmt->executeUpdate(s + studVals[i]);
    }
    std::cout << "STUDENT records inserted." << std::endl;

    s = "create table DEPT(DId int, DName varchar(8))";
    stmt->executeUpdate(s);
    std::cout << "Table DEPT created." << std::endl;

    s = "insert into DEPT(DId, DName) values ";
    std::vector<std::string> deptVals = {"(10, 'compsci')", "(20, 'math')",
                                         "(30, 'drama')"};
    for (int i = 0; i < static_cast<int>(deptVals.size()); i++) {
      stmt->executeUpdate(s + deptVals[i]);
    }
    std::cout << "DEPT records inserted." << std::endl;

    s = "create table COURSE(CId int, Title varchar(20), DeptId int)";
    stmt->executeUpdate(s);
    std::cout << "Table COURSE created" << std::endl;

    s = "insert into COURSE(CId, Title, DeptId) values ";
    std::vector<std::string> courseVals = {
        "(12, 'db systems', 10)", "(22, 'compilers', 10)",
        "(32, 'calculus', 20)",   "(42, 'algebra', 20)",
        "(52, 'acting', 30)",     "(62, 'elocution', 30)"};

    for (int i = 0; i < static_cast<int>(courseVals.size()); i++) {
      stmt->executeUpdate(s + courseVals[i]);
    }
    std::cout << "COURSE records inserted." << std::endl;

    s = "create table SECTION(SectId int, CourseId int, Prof varchar(8), "
        "YearOffered int)";
    stmt->executeUpdate(s);
    std::cout << "Table SECTION created." << std::endl;

    s = "insert into SECTION(SectId, CourseId, Prof, YearOffered) values ";
    std::vector<std::string> sectVals = {
        "(13, 12, 'turing', 2018)", "(23, 12, 'turing', 2019)",
        "(33, 32, 'newton', 2019)", "(43, 32, 'einstein', 2017)",
        "(53, 62, 'brando', 2018)"};

    for (int i = 0; i < static_cast<int>(sectVals.size()); i++) {
      stmt->executeUpdate(s + sectVals[i]);
    }
    std::cout << "SECTION records inserted." << std::endl;

    s = "create table ENROLL(EId int, StudentId int, SectionId int, Grade "
        "varchar(2))";
    stmt->executeUpdate(s);
    std::cout << "Table ENROLL created." << std::endl;

    s = "insert into ENROLL(EId, StudentId, SectionId, Grade) values ";
    std::vector<std::string> enrollVals = {
        "(14, 1, 13, 'A')",  "(24, 1, 43, 'C' )", "(34, 2, 43, 'B+')",
        "(44, 4, 33, 'B' )", "(54, 4, 53, 'A' )", "(64, 6, 53, 'A' )"};

    for (int i = 0; i < static_cast<int>(enrollVals.size()); i++) {
      stmt->executeUpdate(s + enrollVals[i]);
    }

    std::cout << "ENROLL records inserted." << std::endl;
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__ << " on line "
              << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}

void student_major() {
  std::string qry = "";
  qry += "select SName, DName ";
  qry += "from DEPT, STUDENT ";
  qry += "where MajorId = Did";

  std::unique_ptr<DriverAdapter> d = std::make_unique<EmbeddedDriver>();
  try {
    std::unique_ptr<sql::Connection> conn(d->connect(host, username, password));
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> rS(stmt->executeQuery(qry));
    std::cout << "Name\tMajor" << std::endl;
    while (rS->next()) {
      std::string sName = rS->getString("sname");
      std::string dName = rS->getString("dname");
      std::cout << sName << "\t" << dName << std::endl;
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

void find_majors() {
  std::string deptName;
  std::cout << "Enter a department name: ";
  std::cin >> deptName;
  std::cout << "Here are the " + deptName + " majors" << std::endl;
  std::cout << "Name\tGradYear";
  std::string qry = "";
  qry += "select sname, gradyear ";
  qry += "from student, dept ";
  qry += "where did = majorid ";
  qry += "and dname = '" + deptName + "'";

  std::unique_ptr<DriverAdapter> d = std::make_unique<EmbeddedDriver>();
  try {
    std::unique_ptr<sql::Connection> conn(d->connect(host, username, password));
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> rS(stmt->executeQuery(qry));
    while (rS->next()) {
      std::string sName = rS->getString("sname");
      int gradYear = rS->getInt("gradyear");
      std::cout << sName << "\t" << gradYear << std::endl;
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

void change_major() {
  std::string cmd = "";
  cmd += "update STUDENT ";
  cmd += "set majorid=30 ";
  cmd += "where sname = 'amy'";

  std::unique_ptr<DriverAdapter> d = std::make_unique<EmbeddedDriver>();
  try {
    std::unique_ptr<sql::Connection> conn(d->connect(host, username, password));
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    stmt->executeUpdate(cmd);
    std::cout << "Amy is not a drama major." << std::endl;
  } catch (sql::SQLException &e) {
    std::cout << "# ERR: SQLException in " << __FILE__ << " on line "
              << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << " (MySQL error code: " << e.getErrorCode();
    std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
  }
}
} // namespace simpledb

int main() {
  simpledb::create_student_database();
  simpledb::student_major();
  simpledb::change_major();
  simpledb::find_majors();
}

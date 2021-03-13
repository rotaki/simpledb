#include <iostream>

#include "file/page.hpp"
#include "log/logmanager.hpp"
#include "server/simpledb.hpp"
#include "gtest/gtest.h"

namespace simpledb {
void print_log_records(log_manager &lM, const std::string &msg) {
  std::cout << msg << std::endl;
  log_manager::log_iterator iter = lM.iterator();
  while (iter.has_next()) {
    auto rec = std::make_shared<std::vector<char>>(iter.next());
    page p(rec);
    std::string s = p.get_string(0);
    int npos = page::max_length(s.size());
    int val = p.get_int(npos);
    std::cout << "[" << s << ", " << val << "]";
  }
  std::cout << std::endl;
}

std::vector<char> create_log_record(std::string s, int n) {
  int spos = 0;
  int npos = spos + page::max_length(s.size());
  auto b = std::make_shared<std::vector<char>>(npos + sizeof(int));
  page p(b);
  p.set_string(spos, s);
  p.set_int(npos, n);
  return *b;
}

void create_records(log_manager &lM, int start, int end) {
  std::cout << "Creating records: ";
  for (int i = start; i <= end; i++) {
    std::vector<char> rec =
        create_log_record("record" + std::to_string(i), i + 100);
    int lsn = lM.append(rec);
    std::cout << lsn << " ";
  }
  std::cout << std::endl;
}

TEST(log, log_test) {
  simpledb db("logtest", 400, 8);
  log_manager &lM = db.log_mgr();
  print_log_records(lM, "The initial empty log file: ");
  std::cout << "done" << std::endl;
  create_records(lM, 1, 35);
  print_log_records(lM, "The log file now has these records:");
  create_records(lM, 36, 70);
  lM.flush(65);
  print_log_records(lM, "The log file now has these records:");
}
} // namespace simpledb

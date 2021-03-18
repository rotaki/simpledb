#include <iostream>
#include <random>

#include "record/tablescan.hpp"
#include "server/simpledb.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(record, tablescan_test) {
  simpledb db("tabletest", 400, 8);
  auto tx = db.new_tx();
  schema sch;
  sch.add_int_field("A");
  sch.add_string_field("B", 9);
  layout lt(sch);

  for (const std::string &fldName : lt.get_schema().fields()) {
    int offset = lt.offset(fldName);
    std::cout << fldName + " has offset " << offset << std::endl;
  }

  std::cout << "filling the page with 50 random records." << std::endl;
  table_scan tS(tx.get(), "T", lt);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> d(0, 1);

  for (int i = 0; i < 50; i++) {
    tS.insert();
    int n = round(d(gen) * 50);
    tS.set_int("A", n);
    tS.set_string("B", "rec" + std::to_string(n));
    std::cout << "inserting into slot " << tS.get_rid().to_string() << ": {"
              << n << ", rec" << n << "}" << std::endl;
  }

  std::cout << "deleted these records with A-values < 25." << std::endl;
  int count = 0;
  tS.before_first();
  while (tS.next()) {
    int a = tS.get_int("A");
    std::string b = tS.get_string("B");
    if (a < 25) {
      count++;
      std::cout << "slot " << tS.get_rid().to_string() << ": {" << a << ", "
                << b << "}" << std::endl;
      tS.remove();
    }
  }

  std::cout << count << " values under 25 were deleted" << std::endl;
  std::cout << " here are the remaining records" << std::endl;
  tS.before_first();
  while (tS.next()) {
    int a = tS.get_int("A");
    std::string b = tS.get_string("B");
    std::cout << "slot " << tS.get_rid().to_string() << ": {" << a << ", " << b
              << "}" << std::endl;
  }
  tS.close();
  tx->commit();
}
} // namespace simpledb

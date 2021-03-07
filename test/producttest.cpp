#include <iostream>

#include "query/productscan.hpp"
#include "record/layout.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(query, product) {
  simpledb db("producttest");
  auto tx = db.new_tx();
  schema sch1;
  sch1.add_int_field("A");
  sch1.add_string_field("B", 9);
  layout lt1(sch1);
  table_scan tS1(tx.get(), "T1", lt1);

  schema sch2;
  sch2.add_int_field("C");
  sch2.add_string_field("D", 9);
  layout lt2(sch2);
  table_scan tS2(tx.get(), "T2", lt2);

  tS1.before_first();
  int n = 200;
  std::cout << "Inserting " << n << " records into T1." << std::endl;
  for (int i = 0; i < n; i++) {
    tS1.insert();
    tS1.set_int("A", i);
    tS1.set_string("B", "aaa" + std::to_string(i));
  }
  tS1.close();

  tS2.before_first();
  std::cout << "Inserting " << n << " records into T2." << std::endl;
  for (int i = 0; i < n; i++) {
    tS2.insert();
    tS2.set_int("C", n - i - 1);
    tS2.set_string("D", "bbb" + std::to_string(n - i - 1));
  }
  tS2.close();

  auto s1 = std::static_pointer_cast<scan>(
      std::make_shared<table_scan>(tx.get(), "T1", lt1));
  auto s2 = std::static_pointer_cast<scan>(
      std::make_shared<table_scan>(tx.get(), "T2", lt2));
  auto s3 =
      std::static_pointer_cast<scan>(std::make_shared<product_scan>(s1, s2));
  while (s3->next()) {
    std::cout << s3->get_string("B") << std::endl;
  }
  s3->close();
  tx->commit();
}
} // namespace simpledb

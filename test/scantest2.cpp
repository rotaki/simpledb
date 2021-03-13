#include <iostream>
#include <random>

#include "query/productscan.hpp"
#include "query/projectscan.hpp"
#include "query/selectscan.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(query, scantest2_test) {
  simpledb db("scantest2");
  auto tx = db.new_tx();

  schema sch1;
  sch1.add_int_field("A");
  sch1.add_string_field("B", 9);
  layout lt1(sch1);
  auto us1 = std::static_pointer_cast<update_scan>(
      std::make_shared<table_scan>(tx.get(), "T1", lt1));

  us1->before_first();
  int n = 200;
  std::cout << "Inserting " << n << " records into T1." << std::endl;

  for (int i = 0; i < n; i++) {
    us1->insert();
    us1->set_int("A", i);
    us1->set_string("B", "bbb" + std::to_string(i));
  }
  us1->close();

  schema sch2;
  sch2.add_int_field("C");
  sch2.add_string_field("D", 9);
  layout lt2(sch2);
  auto us2 = std::static_pointer_cast<update_scan>(
      std::make_shared<table_scan>(tx.get(), "T2", lt2));
  us2->before_first();
  std::cout << "Inserting " << n << " records into T2." << std::endl;
  for (int i = 0; i < n; i++) {
    us2->insert();
    us2->set_int("C", n - i - 1);
    us2->set_string("D", "ddd" + std::to_string(n - i - 1));
  }
  us2->close();

  auto s1 = std::static_pointer_cast<scan>(
      std::make_shared<table_scan>(tx.get(), "T1", lt1));
  auto s2 = std::static_pointer_cast<scan>(
      std::make_shared<table_scan>(tx.get(), "T2", lt2));
  auto s3 =
      std::static_pointer_cast<scan>(std::make_shared<product_scan>(s1, s2));

  // selecting all records where A=C
  term t(expression("A"), expression("C"));
  predicate pred(t);
  std::cout << "The predicate is " << pred.to_string() << std::endl;
  auto s4 =
      std::static_pointer_cast<scan>(std::make_shared<select_scan>(s3, pred));

  // projecting on [B,D]
  std::vector<std::string> c = {"B", "D"};
  auto s5 =
      std::static_pointer_cast<scan>(std::make_shared<project_scan>(s4, c));
  while (s5->next()) {
    std::cout << s5->get_string("B") << " " << s5->get_string("D") << std::endl;
  }
  s5->close();
  tx->commit();
}
} // namespace simpledb

#include <iostream>
#include <random>

#include "query/scan.hpp"
#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(plan, plannertest1_test) {
  simpledb db("plannertest1");
  auto tx = db.new_tx();
  planner &plnr = db.plnr();
  std::string cmd = "create table T1(A int, B varchar(9))";
  plnr.execute_update(cmd, tx.get());

  int n = 200;
  std::cout << "Inserting " << n << " random records" << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> d(0, 1);

  for (int i = 0; i < n; i++) {
    int a = round(d(gen) * 50);
    std::string b = "rec" + std::to_string(a);
    cmd = "insert into T1(A,B) values (" + std::to_string(a) + ", '" + b + "')";
    plnr.execute_update(cmd, tx.get());
  }

  std::string qry = "select B from T1 where A=10";
  auto p = plnr.create_query_plan(qry, tx.get());
  auto s = p->open();
  while (s->next()) {
    std::cout << s->get_string("b") << std::endl;
  }
  s->close();
  tx->commit();
}
} // namespace simpledb

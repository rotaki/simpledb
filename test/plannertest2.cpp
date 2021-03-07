#include <iostream>
#include <random>

#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "query/scan.hpp"
#include "gtest/gtest.h"

namespace simpledb {
  TEST(plan, plannertest2) {
    simpledb db("plannertest2");
    auto tx = db.new_tx();
    planner* plnr = db.plnr();
    
    std::string cmd = "create table T1(A int, B varchar(9))";
    plnr->execute_update(cmd, tx.get());

    int n = 200;
    std::cout << "Inserting " << n << " records int T1." << std::endl;
    
    for (int i = 0; i < n; i++) {
      int a = i;
      std::string b = "bbb" + std::to_string(a);
      cmd = "insert into T1(A,B) values (" + std::to_string(a) + ", '" + b + "')";
      plnr->execute_update(cmd, tx.get());
    }

    cmd = "create table T2(C int, D varchar(9))";
    plnr->execute_update(cmd, tx.get());
    std::cout << "Inserting " << n << " records int T2." << std::endl;

    for (int i = 0; i < n; i++) {
      int c = n-i-1;
      std::string d = "ddd" + std::to_string(c);
      cmd = "insert into T2(C,D) values(" + std::to_string(c) + ", '" + d + "')";
      plnr->execute_update(cmd, tx.get());
    }
    
    std::string qry = "select B,D from T1,T2 where A=C";
    auto p = plnr->create_query_plan(qry, tx.get());
    auto s = p->open();
    while (s->next()) {
      std::cout << s->get_string("b") << " " << s->get_string("d") << std::endl;
    }
    s->close();
    tx->commit();
  }
}

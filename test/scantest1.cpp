#include <iostream>
#include <random>

#include "server/smartdb.hpp"
#include "tx/transaction.hpp"
#include "query/selectscan.hpp"
#include "query/projectscan.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(query, scantest1) {
    smartdb db("scantest1");
    auto tx = db.new_tx();
    
    schema sch1;
    sch1.add_int_field("A");
    sch1.add_string_field("B", 9);
    layout lt(sch1);
    auto s1 = std::static_pointer_cast<update_scan>(std::make_shared<table_scan>(tx.get(), "T", lt));

    s1->before_first();
    int n = 200;
    std::cout << "Inserting " << n << " random records." << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);

    for (int i = 0; i < n; i++) {
      s1->insert();
      int k = round(d(gen) * 50);
      s1->set_int("A", k);
      s1->set_string("B", "rec"+std::to_string(k));
    }
    s1->close();

    auto s2 = std::static_pointer_cast<scan>(std::make_shared<table_scan>(tx.get(), "T", lt));
    constant c(10);
    term t(expression("A"), expression(c));
    predicate pred(t);

    std::cout << "The predicate is " << pred.to_string() << std::endl;
    auto s3 = std::static_pointer_cast<scan>(std::make_shared<select_scan>(s2, pred));
    std::vector<std::string> fields = {"B"};
    auto s4 = std::static_pointer_cast<scan>(std::make_shared<project_scan>(s3, fields));
    while (s4->next()) {
      std::cout << s4->get_string("B") << std::endl;
    }
    s4->close();
    tx->commit();
  }
}

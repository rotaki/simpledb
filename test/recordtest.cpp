#include <iostream>
#include <random>

#include "server/simpledb.hpp"
#include "record/tablescan.hpp"
#include "gtest/gtest.h"

namespace simpledb {
  TEST(record, record) {
    simpledb db("recordtest", 400, 8);
    auto tx = db.new_tx();
    schema sch;
    sch.add_int_field("A");
    sch.add_string_field("B", 9);
    layout lt(sch);
    
    for (const std::string &fldName: lt.get_schema().fields()) {
      int offset = lt.offset(fldName);
      std::cout << fldName + " has offset " << offset << std::endl;
    }

    block_id blk = tx->append("testfile");
    tx->pin(blk);
    record_page rP(tx.get(), blk, lt);
    rP.format();

    std::cout <<"filling the page with random records." << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);
    
    int slot = rP.insert_after(-1);
    while (slot >= 0) {
      int n = round(d(gen) * 50);
      rP.set_int(slot, "A", n);
      rP.set_string(slot, "B", "rec"+std::to_string(n));
      std::cout << "inserting into slot " << slot << ": {" << n << ", rec" << n << "}" << std::endl;
      slot = rP.insert_after(slot);
    }

    std::cout << "deleted these records with A-values < 25." << std::endl;
    int count = 0;
    slot = rP.next_after(-1);
    while (slot >= 0) {
      int a = rP.get_int(slot, "A");
      std::string b = rP.get_string(slot, "B");
      if (a < 25) {
        count++;
        std::cout << "slot " << slot << ": {" << a << ", " << b <<  "}" << std::endl;
        rP.remove(slot);
      }
      slot = rP.next_after(slot);
    }
    
    std::cout << count << " values under 25 were deleted" << std::endl;
    std::cout << " here are the remaining records" << std::endl;
    slot = rP.next_after(-1);
    while (slot >= 0) {
      int a = rP.get_int(slot, "A");
      std::string b = rP.get_string(slot, "B");
      std::cout << "slot " << slot << ": {" << a << ", " << b << "}" << std::endl;
      slot = rP.next_after(slot);
    }
    tx->unpin(blk);
    tx->commit();
  }
}

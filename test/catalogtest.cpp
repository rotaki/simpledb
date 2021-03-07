#include <iostream>

#include "server/simpledb.hpp"
#include "tx/transaction.hpp"
#include "metadata/tablemanager.hpp"
#include "gtest/gtest.h"

namespace simpledb {
  TEST(metadata, catalogtest) {
    simpledb db("catalogtest", 400, 8);
    auto tx = db.new_tx();
    table_manager tM(true, tx.get());

    schema sch1;
    sch1.add_int_field("A");
    sch1.add_string_field("B", 9);
    tM.create_table("T1", sch1, tx.get());

    schema sch2;
    sch2.add_int_field("C");
    sch2.add_string_field("D", 9);
    tM.create_table("T2", sch2, tx.get());

    layout tCatLayout = tM.get_layout("tblcat", tx.get());
    std::cout << "Here are all the talbes and their lengths." << std::endl;
    table_scan tS1(tx.get(), "tblcat", tCatLayout);
    while (tS1.next()) {
      std::string tName = tS1.get_string("tblname");
      int slotSize = tS1.get_int("slotsize");
      std::cout << tName << " " << slotSize << std::endl;
    }
    tS1.close();

    std::cout << "Here are the fields for each table and their offsets" << std::endl;
    layout fCatLayout = tM.get_layout("fldcat", tx.get());
    table_scan tS2(tx.get(), "fldcat", fCatLayout);
    while (tS2.next()) {
      std::string tName = tS2.get_string("tblname");
      std::string fName = tS2.get_string("fldname");
      int offset = tS2.get_int("offset");
      std::cout << tName << " " << fName << " " << offset << std::endl;
    }
    tS2.close();
    tx->commit();
  }
}

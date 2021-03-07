#include <iostream>
#include <random>

#include "server/smartdb.hpp"
#include "metadata/tablemanager.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(metadata, metadatamanager) {
    smartdb db("metadatamgrtest", 400, 8);
    auto tx = db.new_tx();
    metadata_manager mdM(true, tx.get());
    schema sch;
    sch.add_int_field("A");
    sch.add_string_field("B", 9);

    // Part 1: Table Metadata
    std::string myTable = "MyTable";
    mdM.create_table(myTable, sch, tx.get());
    layout lt = mdM.get_layout(myTable, tx.get());
    int size = lt.slot_size();
    schema sch2 = lt.get_schema();
    std::cout << "MyTable has slot size " << size << std::endl;
    std::cout << "Its fields are: " << std::endl;
    for (const std::string &fldName: sch2.fields()) {
      std::string type;
      if (sch2.type(fldName) == schema::integer) {
        type = "int";
      } else {
        int strLen = sch2.length(fldName);
        type = "varchar(" + std::to_string(strLen) + ")";
      }
      std::cout << fldName << ": " << type << std::endl;
    }

    // Part2: Statistics Metadata
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);

    table_scan tS(tx.get(), myTable, lt);
    for (int i = 0; i < 50; i++) {
      tS.insert();
      int n = round(d(gen) * 50);
      tS.set_int("A", n);
      tS.set_string("B", "rec"+std::to_string(n));
    }

    stat_info sI = mdM.get_stat_info(myTable, lt, tx.get());
    std::cout << "B(MyTable) = " << sI.blocks_accessed() << std::endl;
    std::cout << "R(MyTable) = " << sI.records_output() << std::endl;
    std::cout << "V(MyTable,A) = " << sI.distinct_values("A") << std::endl;
    std::cout << "V(MyTable,B) = "  << sI.distinct_values("B") << std::endl;

    // Part 3:  View MetaData
    std::string viewDef = "select B from MyTable where A = 1";
    mdM.create_view("viewA", viewDef, tx.get());
    std::string v = mdM.get_view_def("viewA", tx.get());
    std::cout << "View def = " + v << std::endl;


    // Part 4: Index Metadata
    mdM.create_index("indexA", "MyTable", "A", tx.get());
    mdM.create_index("indexB", "MyTable", "B", tx.get());
    auto idxMap = mdM.get_index_info("MyTable", tx.get());
    
    auto iI = idxMap.at("A");
    std::cout << "B(indexA) = " <<  iI.blocks_accessed() << std::endl;
    std::cout << "R(indexA) = " << iI.records_output() << std::endl;
    std::cout << "V(indexA,A) = " << iI.distinct_values("A") << std::endl;
    std::cout << "V(indexA,B) = " << iI.distinct_values("B") << std::endl;

    iI = idxMap.at("B");
    std::cout << "B(indexB) = " <<  iI.blocks_accessed() << std::endl;
    std::cout << "R(indexB) = " << iI.records_output() << std::endl;
    std::cout << "V(indexB,A) = " << iI.distinct_values("A") << std::endl;
    std::cout << "V(indexB,B) = " << iI.distinct_values("B") << std::endl;
    tx->commit();
    
  }
}


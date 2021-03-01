#include <random>

#include "tablemanager.hpp"
#include "tablescan.hpp"
#include "indexmanager.hpp"
#include "smartdb.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  TEST(metadatamanager, basic) {
    smartdb db("metadatamgrtest");
    auto tx = db.new_tx();
    std::shared_ptr<metadata_manager> mM(new metadata_manager(true, tx));
    std::shared_ptr<schema> sch(new schema);
    sch->add_int_field("A");
    sch->add_string_field("B", 9);

    std::cout << "part 1: create table metadata" << std::endl;
    mM->create_table("MyTable", sch, tx);
    std::shared_ptr<layout> lt = mM->get_layout("MyTable", tx);
    int size = lt->slot_size();
    auto sch2 = lt->get_schema();
    std::cout << "MyTable has slot size: " << size << std::endl;
    std::cout << "Its fields are: " << std::endl;
    for (const std::string &fldName: sch2->fields()) {
      std::string type;
      if (sch2->type(fldName) == schema::integer) {
        type = "int";
      } else {
        int strlen = sch2->length(fldName);
        type = "varchar(" + std::to_string(strlen) + ")";
      }
      std::cout << fldName << ": " << type << std::endl;
    }

    std::cout << "part 2: statistics metadata" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> d(0, 1);

    std::shared_ptr<table_scan> tS(new table_scan(tx, "MyTable", lt));
    for (int i = 0; i < 50; i++) {
      tS->insert();
      int n = round(d(gen) * 50);
      tS->set_int("A", n);
      tS->set_string("B", "rec"+std::to_string(n));
    }

    std::shared_ptr<stat_info> sI = mM->get_stat_info("MyTable", lt, tx);
    std::cout << "B(MyTable) = " << sI->blocks_accessed() << std::endl;
    std::cout << "R(MyTable) = " << sI->records_output() << std::endl;
    std::cout << "V(MyTable,A) = " << sI->distinct_values("A") << std::endl;
    std::cout << "V(MyTable,B) = "  << sI->distinct_values("B") << std::endl;

    
    std::cout << "part 3: view metadata" << std::endl;
    
    std::string viewDef = "select B from MyTable where A = 1";
    mM->create_view("viewA", viewDef, tx);
    std::string v = mM->get_view_def("viewA", tx);
    std::cout << "View def = " << v << std::endl;

    std::cout << "part 4: index metadata" << std::endl;
    mM->create_index("indexA", "MyTable", "A", tx);
    mM->create_index("indexB", "MyTable", "B", tx);
    auto idxMap = mM->get_index_info("MyTable", tx);
    auto iI = idxMap.at("A");
    std::cout << "B(indexA) = " <<  iI->blocks_accessed() << std::endl;
    std::cout << "R(indexA) = " << iI->records_output() << std::endl;
    std::cout << "V(indexA,A) = " << iI->distinct_values("A") << std::endl;
    std::cout << "V(indexA,B) = " << iI->distinct_values("B") << std::endl;

    iI = idxMap.at("B");
    std::cout << "B(indexB) = " <<  iI->blocks_accessed() << std::endl;
    std::cout << "R(indexB) = " << iI->records_output() << std::endl;
    std::cout << "V(indexB,A) = " << iI->distinct_values("A") << std::endl;
    std::cout << "V(indexB,B) = " << iI->distinct_values("B") << std::endl;
    tx->commit();
    
  }
}

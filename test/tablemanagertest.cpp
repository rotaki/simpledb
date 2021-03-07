#include <iostream>
#include <random>

#include "server/smartdb.hpp"
#include "metadata/tablemanager.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(metadata, tablemanagertest) {
    smartdb db("tblmgrtest", 400, 8);
    auto tx = db.new_tx();
    table_manager tM(true, tx.get());
    schema sch;
    sch.add_int_field("A");
    sch.add_string_field("B", 9);
    tM.create_table("MyTable", sch, tx.get());

    layout lt = tM.get_layout("MyTable", tx.get());
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
    tx->commit();
  }
}

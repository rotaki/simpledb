#include "record/layout.hpp"
#include "gtest/gtest.h"

namespace simpledb {
  TEST(record, layout) {
    schema sch;
    sch.add_int_field("A");
    sch.add_string_field("B", 9);
    layout lt(sch);
    for (const std::string &fldName: lt.get_schema().fields()) {
      int offset = lt.offset(fldName);
      std::cout << fldName << " has offset " << offset << std::endl;
    }
  }
}


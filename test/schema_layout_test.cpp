#include "schema.hpp"
#include "layout.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(schema_layout, basic) {
    std::shared_ptr<schema> sch(new schema);
    sch->add_int_field("cid");
    sch->add_string_field("title", 20);
    sch->add_int_field("deptid");
    layout lt(sch);
    for (const std::string &fldName: lt.get_schema()->fields()) {
      int offset = lt.offset(fldName);
      std::cout << fldName + " has offset " << offset << std::endl;
    }
  }
}

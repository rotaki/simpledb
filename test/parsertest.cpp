#include <iostream>
#include <sstream>

#include "parse/parser.hpp"
#include "gtest/gtest.h"

namespace simpledb {
  TEST(parse, parsertest) {
    std::string s1 = "select    C from      T where A  =B";
    std::string s2 = "insert into   T(A,    B, C)   values('a'    , 'b', 'c')";
    std::string s3 = "delete  from T where A=B";
    std::string s4 = "update T set A='a' where B=C";
    std::string s5 = "create table T(A int, B varchar(9))";
    std::string s6 = "create view V as " + s1;
    std::string s7 = "create index I on T(A)";

    parser p1(s1);
    p1.query();

    parser p2(s2);
    p2.update_cmd();

    parser p3(s3);
    p3.update_cmd();

    parser p4(s4);
    p4.update_cmd();

    parser p5(s5);
    p5.update_cmd();

    parser p6(s6);
    p6.update_cmd();

    parser p7(s7);
    p7.update_cmd();
  }
}

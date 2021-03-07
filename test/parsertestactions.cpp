#include <iostream>
#include <sstream>

#include "parse/parser.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  TEST(parse, parsertestactions) {
    std::string s1 = "select    C from      T where A  =B";
    std::string s2 = "insert into   T(A,    B, C)   values('a'    , 'b', 'c')";
    std::string s3 = "delete  from T where A=B";
    std::string s4 = "update T set A='a' where B=C";
    std::string s5 = "create table T(A int, B varchar(9))";
    std::string s6 = "create view V as " + s1;
    std::string s7 = "create index I on T(A)";

    parser p1(s1);
    std::cout << "Case 1" << std::endl;
    std::cout << "In: " << s1 << std::endl;
    std::cout << "Out: " << p1.query()->to_string() << std::endl;

    parser p2(s2);
    std::cout << "Case 2" << std::endl;
    std::cout << "In: " << s2 << std::endl;
    std::cout << "Out op: " << static_cast<object::operation>(p2.update_cmd()->op()) << std::endl;;

    parser p3(s3);
    std::cout << "Case 3" << std::endl;
    std::cout << "In: " << s3 << std::endl;
    std::cout << "Out op: " << static_cast<object::operation>(p3.update_cmd()->op()) << std::endl;;

    parser p4(s4);
    std::cout << "Case 4" << std::endl;
    std::cout << "In: " << s4 << std::endl;
    std::cout << "Out op: " << static_cast<object::operation>(p4.update_cmd()->op()) << std::endl;;

    parser p5(s5);
    std::cout << "Case 5" << std::endl;
    std::cout << "In: " << s5 << std::endl;
    std::cout << "Out op: " << static_cast<object::operation>(p5.update_cmd()->op()) << std::endl;;

    parser p6(s6);
    std::cout << "Case 6" << std::endl;
    std::cout << "In: " << s6 << std::endl;
    std::cout << "Out op: " << static_cast<object::operation>(p6.update_cmd()->op()) << std::endl;;

    parser p7(s7);
    std::cout << "Case 7" << std::endl;
    std::cout << "In: " << s7 << std::endl;
    std::cout << "Out op: " << static_cast<object::operation>(p7.update_cmd()->op()) << std::endl;;
  }
}

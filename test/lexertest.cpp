#include <iostream>
#include <sstream>

#include "parse/lexer.hpp"
#include "gtest/gtest.h"

namespace simpledb {
TEST(parse, lexer_test) {
  std::string s = "";
  std::stringstream sS("A=1 \n 3=A");
  std::string x = "";
  int y = 0;
  while (std::getline(sS, s, '\n')) {
    lexer lex(s);
    if (lex.match_id()) {
      x = lex.eat_id();
      lex.eat_delim('=');
      y = lex.eat_int_constant();
    } else {
      y = lex.eat_int_constant();
      lex.eat_delim('=');
      x = lex.eat_id();
    }
    std::cout << x << " equals " << y << std::endl;
  }
}
} // namespace simpledb

#include "predparser.hpp"

namespace smartdb {
  pred_parser::pred_parser(const std::string &pS) {
    mLex = std::shared_ptr<lexer>(new lexer(pS));
  }
  
  void pred_parser::field() {
    mLex->eat_id();
  }

  void pred_parser::constant() {
    if (mLex->match_string_constant()) {
      mLex->eat_string_constant();
    } else {
      mLex->eat_int_constant();
    }
  }

  void pred_parser::expression() {
    if (mLex->match_id()) {
      field();
    } else {
      constant();
    }
  }

  void pred_parser::term() {
    expression();
    mLex->eat_delim('=');
    expression();
  }

  void pred_parser::predicate() {
    term();
    if (mLex->match_keyword("and")) {
      mLex->eat_keyword("and");
      predicate();
    }
  }
}

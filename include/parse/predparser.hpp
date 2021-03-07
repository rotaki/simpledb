#pragma once

#include <memory>

#include "parse/lexer.hpp"

namespace simpledb {
  class pred_parser {
  public:
    pred_parser(const std::string &pS);
    void field();
    void constant();
    void expression();
    void term();
    void predicate();
  private:
    lexer mLex;
  };
}

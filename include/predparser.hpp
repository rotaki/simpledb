#pragma once

#include <memory>

#include "lexer.hpp"

namespace smartdb {
  class pred_parser {
  public:
    pred_parser(const std::string &pS);
    void field();
    void constant();
    void expression();
    void term();
    void predicate();
  private:
    std::shared_ptr<lexer> mLex;
  };
}

#pragma once

#include <set>
#include <string>
#include <memory>

#include "parse/streamtokenizer.hpp"

namespace smartdb {
  class lexer {
  public:
    lexer(const std::string &pS);
    bool match_delim(const char &pD);
    bool match_int_constant();
    bool match_string_constant();
    bool match_keyword(const std::string &pW);
    bool match_id();
    void eat_delim(const char &pD);
    int eat_int_constant();
    std::string eat_string_constant();
    void eat_keyword(const std::string &pW);
    std::string eat_id();
  private:
    std::set<std::string> mKeywords;
    std::unique_ptr<stream_tokenizer> mTok;

    void next_token();
    void init_keywords();
  };
}

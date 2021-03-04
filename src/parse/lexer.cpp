#include <sstream>

#include "parse/lexer.hpp"

namespace smartdb {
  lexer::lexer(const std::string &pS) {
    init_keywords();
    std::stringstream sS(pS);
    mTok = std::shared_ptr<stream_tokenizer>(new stream_tokenizer(sS));
    mTok->ordinary_char('.');
    mTok->word_chars('_', '_');
    mTok->lower_case_mode(true);
    next_token();
  }

  bool lexer::match_delim(const char &pD) {
    return pD == static_cast<char>(mTok->mTType);
  }

  bool lexer::match_int_constant() {
    return mTok->mTType == stream_tokenizer::mTT_NUMBER;
  }

  bool lexer::match_string_constant() {
    return '\'' == static_cast<char>(mTok->mTType);
  }

  bool lexer::match_keyword(const std::string &pW) {
    return (mTok->mTType == stream_tokenizer::mTT_WORD) && (mTok->mSVal == pW);
  }

  bool lexer::match_id() {
    return (mTok->mTType == stream_tokenizer::mTT_WORD) && (mKeywords.find(mTok->mSVal) == mKeywords.end());
  }

  // methods to eat the current token
  void lexer::eat_delim(const char &pD) {
    if (!match_delim(pD)) {
      throw std::runtime_error("bad syntax exception");
    }
    next_token();
  }

  int lexer::eat_int_constant() {
    if (!match_int_constant()) {
      throw std::runtime_error("bad syntax exception");
    }
    int i = static_cast<int>(mTok->mNVal);
    next_token();
    return i;
  }

  std::string lexer::eat_string_constant() {
    if (!match_string_constant()) {
      throw std::runtime_error("bad syntax exception");
    }
    std::string s = mTok->mSVal;
    next_token();
    return s;
  }

  void lexer::eat_keyword(const std::string &pW) {
    if (!match_keyword(pW)) {
      throw std::runtime_error("bad syntax exception");
    }
    next_token();
  }

  std::string lexer::eat_id() {
    if (!match_id()) {
      throw std::runtime_error("bad syntax exception");
    }
    std::string s = mTok->mSVal;
    next_token();
    return s;
  }

  void lexer::next_token() {
    try {
      mTok->next_token();
    } catch (std::exception &e) {
      throw std::runtime_error("bad syntax exception");
    }
  }

  void lexer::init_keywords() {
    mKeywords = std::set<std::string>{"select", "from", "where", "and","insert","into", "values", "delete", "update","set", "create", "table", "varchar","int", "view", "as", "index", "on"};
  }
  
}

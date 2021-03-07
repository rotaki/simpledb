#include <iostream>

#include "parse/streamtokenizer.hpp"
#include "gtest/gtest.h"

namespace smartdb {
  void print_current_token(const std::set<std::string> &keywords, stream_tokenizer* tok) {
    
    if (tok->mTType == stream_tokenizer::mTT_NUMBER) {
      std::cout << "IntConstant " << tok->mNVal << std::endl;
    } else if (tok->mTType == stream_tokenizer::mTT_WORD) {
      std::string word = tok->mSVal;
      if (keywords.find(word) != keywords.end()) {
        std::cout << "Keyword " <<  word << std::endl;
      } else {
        std::cout << "Id " << word << std::endl;
      }
    } else if (tok->mTType == '\'') {
      std::cout << "StringConstant " << tok->mSVal << std::endl;
    } else {
      std::cout << "Delimiter " << static_cast<char>(tok->mTType) << std::endl;
    }
  }
  
  TEST(parse, tokenizertest) {
    std::set<std::string> keywords = {"select", "from", "where", "and", "insert", "into", "values", "delete", "update", "set", "create", "table", "int", "varchar", "view", "as", "index", "on"};

    std::string s;
    std::stringstream ss;
    ss << "select a from x, z where b = 3 and c = 'foobar'";

    auto tok = std::make_unique<stream_tokenizer>(ss);
    tok->ordinary_char('.');
    tok->word_chars('_', '_');
    tok->lower_case_mode(true);
    while (tok->next_token() != stream_tokenizer::mTT_EOF) {
      print_current_token(keywords, tok.get());
    }
  }
}

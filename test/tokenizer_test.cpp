#include <random>
#include <iostream>
#include <sstream>

#include "streamtokenizer.hpp"
#include "gtest/gtest.h"

namespace fs = std::filesystem;

namespace smartdb {
  void print_current_token(const std::set<std::string> &pKeywords, std::shared_ptr<stream_tokenizer> pTok) {
    
    if (pTok->mTType == stream_tokenizer::mTT_NUMBER) {
      std::cout << "IntConstant " << pTok->mNVal << std::endl;
    } else if (pTok->mTType == stream_tokenizer::mTT_WORD) {
      std::string word = pTok->mSVal;
      if (pKeywords.find(word) != pKeywords.end()) {
        std::cout << "Keyword " <<  word << std::endl;
      } else {
        std::cout << "Id " << word << std::endl;
      }
    } else if (pTok->mTType == '\'') {
      std::cout << "StringConstant " << pTok->mSVal << std::endl;
    } else {
      std::cout << "Delimiter " << static_cast<char>(pTok->mTType) << std::endl;
    }
  }

  
  TEST(tokenizer, basic) {
    std::set<std::string> keywords = {
                                      "select", "from", "where", "and", "insert", "into", "values", "delete", "update",
                                      "set", "create", "table", "int", "varchar", "view", "as", "index", "on"
    };

    std::string s;
    std::stringstream ss;
    ss << "select a from x, z where b = 3";

    std::shared_ptr<stream_tokenizer> tok(new stream_tokenizer(ss));
    tok->ordinary_char('.');
    tok->word_chars('_', '_');
    tok->lower_case_mode(true);
    while (tok->next_token() != stream_tokenizer::mTT_EOF) {
      print_current_token(keywords, tok);
    }
  }
}
